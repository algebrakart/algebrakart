// Copyright 2016 The SwiftShader Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "VertexRoutine.hpp"

#include "Constants.hpp"
#include "SpirvShader.hpp"
#include "Device/Vertex.hpp"
#include "Device/Renderer.hpp"
#include "Vulkan/VkDebug.hpp"
#include "System/Half.hpp"

namespace sw
{
	VertexRoutine::VertexRoutine(
			const VertexProcessor::State &state,
			vk::PipelineLayout const *pipelineLayout,
			SpirvShader const *spirvShader)
		: routine(pipelineLayout),
		  state(state),
		  spirvShader(spirvShader)
	{
	  	spirvShader->emitProlog(&routine);
	}

	VertexRoutine::~VertexRoutine()
	{
	}

	void VertexRoutine::generate()
	{
		Pointer<Byte> cache = task + OFFSET(VertexTask,vertexCache);
		Pointer<Byte> vertexCache = cache + OFFSET(VertexCache,vertex);
		Pointer<UInt> tagCache = Pointer<UInt>(cache + OFFSET(VertexCache,tag));

		UInt vertexCount = *Pointer<UInt>(task + OFFSET(VertexTask,vertexCount));

		constants = *Pointer<Pointer<Byte>>(data + OFFSET(DrawData,constants));

		// Check the cache one vertex index at a time. If a hit occurs, copy from the cache to the 'vertex' output buffer.
		// On a cache miss, process a SIMD width of consecutive indices from the input batch. They're written to the cache
		// in reverse order to guarantee that the first one doesn't get evicted and can be written out.

		Do
		{
			UInt index = *batch;
			UInt cacheIndex = index & VertexCache::TAG_MASK;

			If(tagCache[cacheIndex] != index)
			{
				readInput(batch);
				program(batch, vertexCount);
				computeClipFlags();

				writeCache(vertexCache, tagCache, batch);
			}

			Pointer<Byte> cacheEntry = vertexCache + cacheIndex * UInt((int)sizeof(Vertex));

			// For points, vertexCount is 1 per primitive, so duplicate vertex for all 3 vertices of the primitive
			for(int i = 0; i < (state.isPoint ? 3 : 1); i++)
			{
				writeVertex(vertex, cacheEntry);
				vertex += sizeof(Vertex);
			}

			batch = Pointer<UInt>(Pointer<Byte>(batch) + sizeof(uint32_t));
			vertexCount--;
		}
		Until(vertexCount == 0)

		Return();
	}

	void VertexRoutine::readInput(Pointer<UInt> &batch)
	{
		for(int i = 0; i < MAX_INTERFACE_COMPONENTS; i += 4)
		{
			if(spirvShader->inputs[i + 0].Type != SpirvShader::ATTRIBTYPE_UNUSED ||
			   spirvShader->inputs[i + 1].Type != SpirvShader::ATTRIBTYPE_UNUSED ||
			   spirvShader->inputs[i + 2].Type != SpirvShader::ATTRIBTYPE_UNUSED ||
			   spirvShader->inputs[i + 3].Type != SpirvShader::ATTRIBTYPE_UNUSED)
			{
				Pointer<Byte> input = *Pointer<Pointer<Byte>>(data + OFFSET(DrawData, input) + sizeof(void*) * (i / 4));
				UInt stride = *Pointer<UInt>(data + OFFSET(DrawData, stride) + sizeof(uint32_t) * (i / 4));
				UInt robustnessSize(0);
				if(state.robustBufferAccess)
				{
					robustnessSize = *Pointer<UInt>(data + OFFSET(DrawData, robustnessSize) + sizeof(uint32_t) * (i / 4));
				}

				auto value = readStream(input, stride, state.input[i / 4], batch, state.robustBufferAccess, robustnessSize);
				routine.inputs[i + 0] = value.x;
				routine.inputs[i + 1] = value.y;
				routine.inputs[i + 2] = value.z;
				routine.inputs[i + 3] = value.w;
			}
		}
	}

	void VertexRoutine::computeClipFlags()
	{
		auto it = spirvShader->outputBuiltins.find(spv::BuiltInPosition);
		assert(it != spirvShader->outputBuiltins.end());
		assert(it->second.SizeInComponents == 4);
		auto &pos = routine.getVariable(it->second.Id);
		auto posX = pos[it->second.FirstComponent + 0];
		auto posY = pos[it->second.FirstComponent + 1];
		auto posZ = pos[it->second.FirstComponent + 2];
		auto posW = pos[it->second.FirstComponent + 3];

		Int4 maxX = CmpLT(posW, posX);
		Int4 maxY = CmpLT(posW, posY);
		Int4 maxZ = CmpLT(posW, posZ);
		Int4 minX = CmpNLE(-posW, posX);
		Int4 minY = CmpNLE(-posW, posY);
		Int4 minZ = CmpNLE(Float4(0.0f), posZ);

		clipFlags =  Pointer<Int>(constants + OFFSET(Constants,maxX))[SignMask(maxX)];
		clipFlags |= Pointer<Int>(constants + OFFSET(Constants,maxY))[SignMask(maxY)];
		clipFlags |= Pointer<Int>(constants + OFFSET(Constants,maxZ))[SignMask(maxZ)];
		clipFlags |= Pointer<Int>(constants + OFFSET(Constants,minX))[SignMask(minX)];
		clipFlags |= Pointer<Int>(constants + OFFSET(Constants,minY))[SignMask(minY)];
		clipFlags |= Pointer<Int>(constants + OFFSET(Constants,minZ))[SignMask(minZ)];

		Int4 finiteX = CmpLE(Abs(posX), *Pointer<Float4>(constants + OFFSET(Constants,maxPos)));
		Int4 finiteY = CmpLE(Abs(posY), *Pointer<Float4>(constants + OFFSET(Constants,maxPos)));
		Int4 finiteZ = CmpLE(Abs(posZ), *Pointer<Float4>(constants + OFFSET(Constants,maxPos)));

		Int4 finiteXYZ = finiteX & finiteY & finiteZ;
		clipFlags |= Pointer<Int>(constants + OFFSET(Constants,fini))[SignMask(finiteXYZ)];
	}

	Vector4f VertexRoutine::readStream(Pointer<Byte> &buffer, UInt &stride, const Stream &stream, Pointer<UInt> &batch,
	                                   bool robustBufferAccess, UInt & robustnessSize)
	{
		Vector4f v;
		UInt4 offsets = *Pointer<UInt4>(As<Pointer<UInt4>>(batch)) * UInt4(stride);

		Pointer<Byte> source0 = buffer + offsets.x;
		Pointer<Byte> source1 = buffer + offsets.y;
		Pointer<Byte> source2 = buffer + offsets.z;
		Pointer<Byte> source3 = buffer + offsets.w;

		UInt4 zero(0);
		if (robustBufferAccess)
		{
			// TODO(b/141124876): Optimize for wide-vector gather operations.
			UInt4 limits = offsets + UInt4(stream.bytesPerAttrib());
			Pointer<Byte> zeroSource = As<Pointer<Byte>>(&zero);
			source0 = IfThenElse(limits.x <= robustnessSize, source0, zeroSource);
			source1 = IfThenElse(limits.y <= robustnessSize, source1, zeroSource);
			source2 = IfThenElse(limits.z <= robustnessSize, source2, zeroSource);
			source3 = IfThenElse(limits.w <= robustnessSize, source3, zeroSource);
		}

		bool isNativeFloatAttrib = (stream.attribType == SpirvShader::ATTRIBTYPE_FLOAT) || stream.normalized;

		switch(stream.type)
		{
		case STREAMTYPE_FLOAT:
			{
				if(stream.count == 0)
				{
					// Null stream, all default components
				}
				else
				{
					if(stream.count == 1)
					{
						v.x.x = *Pointer<Float>(source0);
						v.x.y = *Pointer<Float>(source1);
						v.x.z = *Pointer<Float>(source2);
						v.x.w = *Pointer<Float>(source3);
					}
					else
					{
						v.x = *Pointer<Float4>(source0);
						v.y = *Pointer<Float4>(source1);
						v.z = *Pointer<Float4>(source2);
						v.w = *Pointer<Float4>(source3);

						transpose4xN(v.x, v.y, v.z, v.w, stream.count);
					}

					switch(stream.attribType)
					{
					case SpirvShader::ATTRIBTYPE_INT:
						if(stream.count >= 1) v.x = As<Float4>(Int4(v.x));
						if(stream.count >= 2) v.x = As<Float4>(Int4(v.y));
						if(stream.count >= 3) v.x = As<Float4>(Int4(v.z));
						if(stream.count >= 4) v.x = As<Float4>(Int4(v.w));
						break;
					case SpirvShader::ATTRIBTYPE_UINT:
						if(stream.count >= 1) v.x = As<Float4>(UInt4(v.x));
						if(stream.count >= 2) v.x = As<Float4>(UInt4(v.y));
						if(stream.count >= 3) v.x = As<Float4>(UInt4(v.z));
						if(stream.count >= 4) v.x = As<Float4>(UInt4(v.w));
						break;
					default:
						break;
					}
				}
			}
			break;
		case STREAMTYPE_BYTE:
			if(isNativeFloatAttrib) // Stream: UByte, Shader attrib: Float
			{
				v.x = Float4(*Pointer<Byte4>(source0));
				v.y = Float4(*Pointer<Byte4>(source1));
				v.z = Float4(*Pointer<Byte4>(source2));
				v.w = Float4(*Pointer<Byte4>(source3));

				transpose4xN(v.x, v.y, v.z, v.w, stream.count);

				if(stream.normalized)
				{
					if(stream.count >= 1) v.x *= *Pointer<Float4>(constants + OFFSET(Constants,unscaleByte));
					if(stream.count >= 2) v.y *= *Pointer<Float4>(constants + OFFSET(Constants,unscaleByte));
					if(stream.count >= 3) v.z *= *Pointer<Float4>(constants + OFFSET(Constants,unscaleByte));
					if(stream.count >= 4) v.w *= *Pointer<Float4>(constants + OFFSET(Constants,unscaleByte));
				}
			}
			else // Stream: UByte, Shader attrib: Int / UInt
			{
				v.x = As<Float4>(Int4(*Pointer<Byte4>(source0)));
				v.y = As<Float4>(Int4(*Pointer<Byte4>(source1)));
				v.z = As<Float4>(Int4(*Pointer<Byte4>(source2)));
				v.w = As<Float4>(Int4(*Pointer<Byte4>(source3)));

				transpose4xN(v.x, v.y, v.z, v.w, stream.count);
			}
			break;
		case STREAMTYPE_SBYTE:
			if(isNativeFloatAttrib) // Stream: SByte, Shader attrib: Float
			{
				v.x = Float4(*Pointer<SByte4>(source0));
				v.y = Float4(*Pointer<SByte4>(source1));
				v.z = Float4(*Pointer<SByte4>(source2));
				v.w = Float4(*Pointer<SByte4>(source3));

				transpose4xN(v.x, v.y, v.z, v.w, stream.count);

				if(stream.normalized)
				{
					if(stream.count >= 1) v.x *= *Pointer<Float4>(constants + OFFSET(Constants,unscaleSByte));
					if(stream.count >= 2) v.y *= *Pointer<Float4>(constants + OFFSET(Constants,unscaleSByte));
					if(stream.count >= 3) v.z *= *Pointer<Float4>(constants + OFFSET(Constants,unscaleSByte));
					if(stream.count >= 4) v.w *= *Pointer<Float4>(constants + OFFSET(Constants,unscaleSByte));
				}
			}
			else // Stream: SByte, Shader attrib: Int / UInt
			{
				v.x = As<Float4>(Int4(*Pointer<SByte4>(source0)));
				v.y = As<Float4>(Int4(*Pointer<SByte4>(source1)));
				v.z = As<Float4>(Int4(*Pointer<SByte4>(source2)));
				v.w = As<Float4>(Int4(*Pointer<SByte4>(source3)));

				transpose4xN(v.x, v.y, v.z, v.w, stream.count);
			}
			break;
		case STREAMTYPE_COLOR:
			{
				v.x = Float4(*Pointer<Byte4>(source0)) * *Pointer<Float4>(constants + OFFSET(Constants,unscaleByte));
				v.y = Float4(*Pointer<Byte4>(source1)) * *Pointer<Float4>(constants + OFFSET(Constants,unscaleByte));
				v.z = Float4(*Pointer<Byte4>(source2)) * *Pointer<Float4>(constants + OFFSET(Constants,unscaleByte));
				v.w = Float4(*Pointer<Byte4>(source3)) * *Pointer<Float4>(constants + OFFSET(Constants,unscaleByte));

				transpose4x4(v.x, v.y, v.z, v.w);

				// Swap red and blue
				Float4 t = v.x;
				v.x = v.z;
				v.z = t;
			}
			break;
		case STREAMTYPE_SHORT:
			if(isNativeFloatAttrib) // Stream: Int, Shader attrib: Float
			{
				v.x = Float4(*Pointer<Short4>(source0));
				v.y = Float4(*Pointer<Short4>(source1));
				v.z = Float4(*Pointer<Short4>(source2));
				v.w = Float4(*Pointer<Short4>(source3));

				transpose4xN(v.x, v.y, v.z, v.w, stream.count);

				if(stream.normalized)
				{
					if(stream.count >= 1) v.x *= *Pointer<Float4>(constants + OFFSET(Constants,unscaleShort));
					if(stream.count >= 2) v.y *= *Pointer<Float4>(constants + OFFSET(Constants,unscaleShort));
					if(stream.count >= 3) v.z *= *Pointer<Float4>(constants + OFFSET(Constants,unscaleShort));
					if(stream.count >= 4) v.w *= *Pointer<Float4>(constants + OFFSET(Constants,unscaleShort));
				}
			}
			else // Stream: Short, Shader attrib: Int/UInt, no type conversion
			{
				v.x = As<Float4>(Int4(*Pointer<Short4>(source0)));
				v.y = As<Float4>(Int4(*Pointer<Short4>(source1)));
				v.z = As<Float4>(Int4(*Pointer<Short4>(source2)));
				v.w = As<Float4>(Int4(*Pointer<Short4>(source3)));

				transpose4xN(v.x, v.y, v.z, v.w, stream.count);
			}
			break;
		case STREAMTYPE_USHORT:
			if(isNativeFloatAttrib) // Stream: Int, Shader attrib: Float
			{
				v.x = Float4(*Pointer<UShort4>(source0));
				v.y = Float4(*Pointer<UShort4>(source1));
				v.z = Float4(*Pointer<UShort4>(source2));
				v.w = Float4(*Pointer<UShort4>(source3));

				transpose4xN(v.x, v.y, v.z, v.w, stream.count);

				if(stream.normalized)
				{
					if(stream.count >= 1) v.x *= *Pointer<Float4>(constants + OFFSET(Constants,unscaleUShort));
					if(stream.count >= 2) v.y *= *Pointer<Float4>(constants + OFFSET(Constants,unscaleUShort));
					if(stream.count >= 3) v.z *= *Pointer<Float4>(constants + OFFSET(Constants,unscaleUShort));
					if(stream.count >= 4) v.w *= *Pointer<Float4>(constants + OFFSET(Constants,unscaleUShort));
				}
			}
			else // Stream: UShort, Shader attrib: Int/UInt, no type conversion
			{
				v.x = As<Float4>(Int4(*Pointer<UShort4>(source0)));
				v.y = As<Float4>(Int4(*Pointer<UShort4>(source1)));
				v.z = As<Float4>(Int4(*Pointer<UShort4>(source2)));
				v.w = As<Float4>(Int4(*Pointer<UShort4>(source3)));

				transpose4xN(v.x, v.y, v.z, v.w, stream.count);
			}
			break;
		case STREAMTYPE_INT:
			if(isNativeFloatAttrib) // Stream: Int, Shader attrib: Float
			{
				v.x = Float4(*Pointer<Int4>(source0));
				v.y = Float4(*Pointer<Int4>(source1));
				v.z = Float4(*Pointer<Int4>(source2));
				v.w = Float4(*Pointer<Int4>(source3));

				transpose4xN(v.x, v.y, v.z, v.w, stream.count);

				if(stream.normalized)
				{
					if(stream.count >= 1) v.x *= *Pointer<Float4>(constants + OFFSET(Constants, unscaleInt));
					if(stream.count >= 2) v.y *= *Pointer<Float4>(constants + OFFSET(Constants, unscaleInt));
					if(stream.count >= 3) v.z *= *Pointer<Float4>(constants + OFFSET(Constants, unscaleInt));
					if(stream.count >= 4) v.w *= *Pointer<Float4>(constants + OFFSET(Constants, unscaleInt));
				}
			}
			else // Stream: Int, Shader attrib: Int/UInt, no type conversion
			{
				v.x = *Pointer<Float4>(source0);
				v.y = *Pointer<Float4>(source1);
				v.z = *Pointer<Float4>(source2);
				v.w = *Pointer<Float4>(source3);

				transpose4xN(v.x, v.y, v.z, v.w, stream.count);
			}
			break;
		case STREAMTYPE_UINT:
			if(isNativeFloatAttrib) // Stream: UInt, Shader attrib: Float
			{
				v.x = Float4(*Pointer<UInt4>(source0));
				v.y = Float4(*Pointer<UInt4>(source1));
				v.z = Float4(*Pointer<UInt4>(source2));
				v.w = Float4(*Pointer<UInt4>(source3));

				transpose4xN(v.x, v.y, v.z, v.w, stream.count);

				if(stream.normalized)
				{
					if(stream.count >= 1) v.x *= *Pointer<Float4>(constants + OFFSET(Constants, unscaleUInt));
					if(stream.count >= 2) v.y *= *Pointer<Float4>(constants + OFFSET(Constants, unscaleUInt));
					if(stream.count >= 3) v.z *= *Pointer<Float4>(constants + OFFSET(Constants, unscaleUInt));
					if(stream.count >= 4) v.w *= *Pointer<Float4>(constants + OFFSET(Constants, unscaleUInt));
				}
			}
			else // Stream: UInt, Shader attrib: Int/UInt, no type conversion
			{
				v.x = *Pointer<Float4>(source0);
				v.y = *Pointer<Float4>(source1);
				v.z = *Pointer<Float4>(source2);
				v.w = *Pointer<Float4>(source3);

				transpose4xN(v.x, v.y, v.z, v.w, stream.count);
			}
			break;
		case STREAMTYPE_HALF:
			{
				if(stream.count >= 1)
				{
					UShort x0 = *Pointer<UShort>(source0 + 0);
					UShort x1 = *Pointer<UShort>(source1 + 0);
					UShort x2 = *Pointer<UShort>(source2 + 0);
					UShort x3 = *Pointer<UShort>(source3 + 0);

					v.x.x = *Pointer<Float>(constants + OFFSET(Constants,half2float) + Int(x0) * 4);
					v.x.y = *Pointer<Float>(constants + OFFSET(Constants,half2float) + Int(x1) * 4);
					v.x.z = *Pointer<Float>(constants + OFFSET(Constants,half2float) + Int(x2) * 4);
					v.x.w = *Pointer<Float>(constants + OFFSET(Constants,half2float) + Int(x3) * 4);
				}

				if(stream.count >= 2)
				{
					UShort y0 = *Pointer<UShort>(source0 + 2);
					UShort y1 = *Pointer<UShort>(source1 + 2);
					UShort y2 = *Pointer<UShort>(source2 + 2);
					UShort y3 = *Pointer<UShort>(source3 + 2);

					v.y.x = *Pointer<Float>(constants + OFFSET(Constants,half2float) + Int(y0) * 4);
					v.y.y = *Pointer<Float>(constants + OFFSET(Constants,half2float) + Int(y1) * 4);
					v.y.z = *Pointer<Float>(constants + OFFSET(Constants,half2float) + Int(y2) * 4);
					v.y.w = *Pointer<Float>(constants + OFFSET(Constants,half2float) + Int(y3) * 4);
				}

				if(stream.count >= 3)
				{
					UShort z0 = *Pointer<UShort>(source0 + 4);
					UShort z1 = *Pointer<UShort>(source1 + 4);
					UShort z2 = *Pointer<UShort>(source2 + 4);
					UShort z3 = *Pointer<UShort>(source3 + 4);

					v.z.x = *Pointer<Float>(constants + OFFSET(Constants,half2float) + Int(z0) * 4);
					v.z.y = *Pointer<Float>(constants + OFFSET(Constants,half2float) + Int(z1) * 4);
					v.z.z = *Pointer<Float>(constants + OFFSET(Constants,half2float) + Int(z2) * 4);
					v.z.w = *Pointer<Float>(constants + OFFSET(Constants,half2float) + Int(z3) * 4);
				}

				if(stream.count >= 4)
				{
					UShort w0 = *Pointer<UShort>(source0 + 6);
					UShort w1 = *Pointer<UShort>(source1 + 6);
					UShort w2 = *Pointer<UShort>(source2 + 6);
					UShort w3 = *Pointer<UShort>(source3 + 6);

					v.w.x = *Pointer<Float>(constants + OFFSET(Constants,half2float) + Int(w0) * 4);
					v.w.y = *Pointer<Float>(constants + OFFSET(Constants,half2float) + Int(w1) * 4);
					v.w.z = *Pointer<Float>(constants + OFFSET(Constants,half2float) + Int(w2) * 4);
					v.w.w = *Pointer<Float>(constants + OFFSET(Constants,half2float) + Int(w3) * 4);
				}
			}
			break;
		case STREAMTYPE_2_10_10_10_INT:
			{
				Int4 src;
				src = Insert(src, *Pointer<Int>(source0), 0);
				src = Insert(src, *Pointer<Int>(source1), 1);
				src = Insert(src, *Pointer<Int>(source2), 2);
				src = Insert(src, *Pointer<Int>(source3), 3);

				v.x = Float4((src << 22) >> 22);
				v.y = Float4((src << 12) >> 22);
				v.z = Float4((src << 02) >> 22);
				v.w = Float4(src >> 30);

				if(stream.normalized)
				{
					v.x = Max(v.x * Float4(1.0f / 0x1FF), Float4(-1.0f));
					v.y = Max(v.y * Float4(1.0f / 0x1FF), Float4(-1.0f));
					v.z = Max(v.z * Float4(1.0f / 0x1FF), Float4(-1.0f));
					v.w = Max(v.w, Float4(-1.0f));
				}
			}
			break;
		case STREAMTYPE_2_10_10_10_UINT:
			{
				Int4 src;
				src = Insert(src, *Pointer<Int>(source0), 0);
				src = Insert(src, *Pointer<Int>(source1), 1);
				src = Insert(src, *Pointer<Int>(source2), 2);
				src = Insert(src, *Pointer<Int>(source3), 3);

				v.x = Float4(src & Int4(0x3FF));
				v.y = Float4((src >> 10) & Int4(0x3FF));
				v.z = Float4((src >> 20) & Int4(0x3FF));
				v.w = Float4((src >> 30) & Int4(0x3));

				if(stream.normalized)
				{
					v.x *= Float4(1.0f / 0x3FF);
					v.y *= Float4(1.0f / 0x3FF);
					v.z *= Float4(1.0f / 0x3FF);
					v.w *= Float4(1.0f / 0x3);
				}
			}
			break;
		default:
			UNSUPPORTED("stream.type %d", int(stream.type));
		}

		if(stream.count < 1) v.x = Float4(0.0f);
		if(stream.count < 2) v.y = Float4(0.0f);
		if(stream.count < 3) v.z = Float4(0.0f);
		if(stream.count < 4) v.w = isNativeFloatAttrib ? As<Float4>(Float4(1.0f)) : As<Float4>(Int4(1));

		return v;
	}

	void VertexRoutine::writeCache(Pointer<Byte> &vertexCache, Pointer<UInt> &tagCache, Pointer<UInt> &batch)
	{
		UInt index0 = batch[0];
		UInt index1 = batch[1];
		UInt index2 = batch[2];
		UInt index3 = batch[3];

		UInt cacheIndex0 = index0 & VertexCache::TAG_MASK;
		UInt cacheIndex1 = index1 & VertexCache::TAG_MASK;
		UInt cacheIndex2 = index2 & VertexCache::TAG_MASK;
		UInt cacheIndex3 = index3 & VertexCache::TAG_MASK;

		// We processed a SIMD group of vertices, with the first one being the one that missed the cache tag check.
		// Write them out in reverse order here and below to ensure the first one is now guaranteed to be in the cache.
		tagCache[cacheIndex3] = index3;
		tagCache[cacheIndex2] = index2;
		tagCache[cacheIndex1] = index1;
		tagCache[cacheIndex0] = index0;

		auto it = spirvShader->outputBuiltins.find(spv::BuiltInPosition);
		assert(it != spirvShader->outputBuiltins.end());
		assert(it->second.SizeInComponents == 4);
		auto &position = routine.getVariable(it->second.Id);

		Vector4f pos;
		pos.x = position[it->second.FirstComponent + 0];
		pos.y = position[it->second.FirstComponent + 1];
		pos.z = position[it->second.FirstComponent + 2];
		pos.w = position[it->second.FirstComponent + 3];

		// Projection and viewport transform.
		Float4 w = As<Float4>(As<Int4>(pos.w) | (As<Int4>(CmpEQ(pos.w, Float4(0.0f))) & As<Int4>(Float4(1.0f))));
		Float4 rhw = Float4(1.0f) / w;

		Vector4f proj;
		proj.x = As<Float4>(RoundInt(*Pointer<Float4>(data + OFFSET(DrawData,X0x16)) + pos.x * rhw * *Pointer<Float4>(data + OFFSET(DrawData,Wx16))));
		proj.y = As<Float4>(RoundInt(*Pointer<Float4>(data + OFFSET(DrawData,Y0x16)) + pos.y * rhw * *Pointer<Float4>(data + OFFSET(DrawData,Hx16))));
		proj.z = pos.z * rhw;
		proj.w = rhw;

		transpose4x4(pos.x, pos.y, pos.z, pos.w);

		*Pointer<Float4>(vertexCache + sizeof(Vertex) * cacheIndex3 + OFFSET(Vertex,position), 16) = pos.w;
		*Pointer<Float4>(vertexCache + sizeof(Vertex) * cacheIndex2 + OFFSET(Vertex,position), 16) = pos.z;
		*Pointer<Float4>(vertexCache + sizeof(Vertex) * cacheIndex1 + OFFSET(Vertex,position), 16) = pos.y;
		*Pointer<Float4>(vertexCache + sizeof(Vertex) * cacheIndex0 + OFFSET(Vertex,position), 16) = pos.x;

		it = spirvShader->outputBuiltins.find(spv::BuiltInPointSize);
		if(it != spirvShader->outputBuiltins.end())
		{
			assert(it->second.SizeInComponents == 1);
			auto psize = routine.getVariable(it->second.Id)[it->second.FirstComponent];

			*Pointer<Float>(vertexCache + sizeof(Vertex) * cacheIndex3 + OFFSET(Vertex,pointSize)) = Extract(psize, 3);
			*Pointer<Float>(vertexCache + sizeof(Vertex) * cacheIndex2 + OFFSET(Vertex,pointSize)) = Extract(psize, 2);
			*Pointer<Float>(vertexCache + sizeof(Vertex) * cacheIndex1 + OFFSET(Vertex,pointSize)) = Extract(psize, 1);
			*Pointer<Float>(vertexCache + sizeof(Vertex) * cacheIndex0 + OFFSET(Vertex,pointSize)) = Extract(psize, 0);
		}

		*Pointer<Int>(vertexCache + sizeof(Vertex) * cacheIndex3 + OFFSET(Vertex,clipFlags)) = (clipFlags >> 24) & 0x0000000FF;
		*Pointer<Int>(vertexCache + sizeof(Vertex) * cacheIndex2 + OFFSET(Vertex,clipFlags)) = (clipFlags >> 16) & 0x0000000FF;
		*Pointer<Int>(vertexCache + sizeof(Vertex) * cacheIndex1 + OFFSET(Vertex,clipFlags)) = (clipFlags >> 8)  & 0x0000000FF;
		*Pointer<Int>(vertexCache + sizeof(Vertex) * cacheIndex0 + OFFSET(Vertex,clipFlags)) = (clipFlags >> 0)  & 0x0000000FF;

		transpose4x4(proj.x, proj.y, proj.z, proj.w);

		*Pointer<Float4>(vertexCache + sizeof(Vertex) * cacheIndex3 + OFFSET(Vertex,projected), 16) = proj.w;
		*Pointer<Float4>(vertexCache + sizeof(Vertex) * cacheIndex2 + OFFSET(Vertex,projected), 16) = proj.z;
		*Pointer<Float4>(vertexCache + sizeof(Vertex) * cacheIndex1 + OFFSET(Vertex,projected), 16) = proj.y;
		*Pointer<Float4>(vertexCache + sizeof(Vertex) * cacheIndex0 + OFFSET(Vertex,projected), 16) = proj.x;

		for(int i = 0; i < MAX_INTERFACE_COMPONENTS; i += 4)
		{
			if(spirvShader->outputs[i + 0].Type != SpirvShader::ATTRIBTYPE_UNUSED ||
			   spirvShader->outputs[i + 1].Type != SpirvShader::ATTRIBTYPE_UNUSED ||
			   spirvShader->outputs[i + 2].Type != SpirvShader::ATTRIBTYPE_UNUSED ||
			   spirvShader->outputs[i + 3].Type != SpirvShader::ATTRIBTYPE_UNUSED)
			{
				Vector4f v;
				v.x = routine.outputs[i + 0];
				v.y = routine.outputs[i + 1];
				v.z = routine.outputs[i + 2];
				v.w = routine.outputs[i + 3];

				transpose4x4(v.x, v.y, v.z, v.w);

				*Pointer<Float4>(vertexCache + sizeof(Vertex) * cacheIndex3 + OFFSET(Vertex,v[i]), 16) = v.w;
				*Pointer<Float4>(vertexCache + sizeof(Vertex) * cacheIndex2 + OFFSET(Vertex,v[i]), 16) = v.z;
				*Pointer<Float4>(vertexCache + sizeof(Vertex) * cacheIndex1 + OFFSET(Vertex,v[i]), 16) = v.y;
				*Pointer<Float4>(vertexCache + sizeof(Vertex) * cacheIndex0 + OFFSET(Vertex,v[i]), 16) = v.x;
			}
		}
	}

	void VertexRoutine::writeVertex(const Pointer<Byte> &vertex, Pointer<Byte> &cacheEntry)
	{
		*Pointer<Int4>(vertex + OFFSET(Vertex,position)) = *Pointer<Int4>(cacheEntry + OFFSET(Vertex,position));
		*Pointer<Int>(vertex + OFFSET(Vertex,pointSize)) = *Pointer<Int>(cacheEntry + OFFSET(Vertex,pointSize));

		*Pointer<Int>(vertex + OFFSET(Vertex,clipFlags)) = *Pointer<Int>(cacheEntry + OFFSET(Vertex,clipFlags));
		*Pointer<Int4>(vertex + OFFSET(Vertex,projected)) = *Pointer<Int4>(cacheEntry + OFFSET(Vertex,projected));

		for(int i = 0; i < MAX_INTERFACE_COMPONENTS; i++)
		{
			if(spirvShader->outputs[i].Type != SpirvShader::ATTRIBTYPE_UNUSED)
			{
				*Pointer<Int>(vertex + OFFSET(Vertex, v[i]), 4) = *Pointer<Int>(cacheEntry + OFFSET(Vertex, v[i]), 4);
			}
		}
	}
}
