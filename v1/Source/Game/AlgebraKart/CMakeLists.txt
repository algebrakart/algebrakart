#
# Copyright (c) 2024 Bad Zindagi Music
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

# Set project name
project (AlgebraKart)

# Enable ODBC Database (Server logging)
set(ODBC_INCLUDE_DIRS "/usr/include/")
set (URHO3D_DATABASE_ODBC 1)

# Find Urho3D library
find_package (Urho3D REQUIRED)
include_directories (${URHO3D_INCLUDE_DIRS})

# Define target name
set (TARGET_NAME AlgebraKart)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CUDA_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


set (BEAT_FILES beat/BeatTime.cpp beat/BeatTime.h beat/Beat.cpp beat/Beat.h beat/Sequencer.cpp beat/Sequencer.h beat/Sampler.cpp beat/Sampler.h beat/Synthesizer.cpp beat/Synthesizer.h beat/Recorder.cpp beat/Recorder.h beat/BufferData.cpp beat/BufferData.h beat/AudioSpectrumAnalyzer.h beat/AudioSpectrumRender.h beat/AudioSpectrumAxis.h beat/AudioSpectrumCapture.h)
set (NETWORK_FILES network/Server.cpp network/Server.h network/ClientObj.cpp network/ClientObj.h network/NetworkActor.cpp network/NetworkActor.h network/csp/CSP_Client.cpp network/csp/CSP_Client.h network/csp/CSP_Server.cpp network/csp/CSP_Server.h network/csp/CSP_messages.h network/csp/StateSnapshot.h)
define_source_files (EXTRA_H_FILES ${NETWORK_FILES} ${BEAT_FILES} PixelImage.cpp PixelImage.h Missile.cpp Missile.h Pickup.cpp Pickup.h PickupFactory.cpp PickupFactory.h WheelTrackModel.cpp WheelTrackModel.h SFXModel.cpp SFXModel.h network/Vehicle.cpp network/Vehicle.h SmoothStep.cpp SmoothStep.h ai/genotype.cpp ai/genotype.h util/random_d.h ai/genetic_algorithm.cpp ai/genetic_algorithm.h ai/evolution_manager.cpp ai/evolution_manager.h ai/agent.cpp ai/agent.h ai/neural_layer.cpp ai/neural_layer.h ai/neural_network.cpp ai/neural_network.h util/math_helper.cpp util/math_helper.h util/event.cpp util/event.h ai/agent_controller.cpp ai/agent_controller.h shared_libs.h ai/sensor.cpp ai/sensor.h ai/flag.cpp ai/flag.h ai/agent_movement.cpp ai/agent_movement.h ai/fsm_event_data.cpp ai/fsm_event_data.h ai/fsm.cpp ai/fsm.h util/semaphore.h ai/agent_fsm.cpp ai/agent_fsm.h util/event.cpp util/event.h types.h util/file_utility.cpp util/file_utility.h Constants.h ViewCone.cpp ViewCone.h GenomeVisualization.cpp GenomeVisualization.h VehicleReplaySystem.cpp VehicleReplaySystem.h)

# Setup target with resource copying
setup_main_executable ()

# Setup test cases
setup_test ()
target_link_libraries (AlgebraKart muFFT)