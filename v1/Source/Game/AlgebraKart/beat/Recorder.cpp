#include "Recorder.h"

#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>


#ifdef MAYASCAPE_SERVER
#include <Urho3D/Database/Database.h>
#include <Urho3D/Database/DatabaseEvents.h>
#else
// Skip Urho3D database defs for client
#endif



//=============================================================================
// RECORDER
//=============================================================================

void Recorder::RegisterObject(Context *context)
{
    context->RegisterFactory<Recorder>();
}

Recorder::Recorder(Context *context) : Object(context), currSeqId_(0) {

#ifdef MAYASCAPE_SERVER

    String dsn = "MAYASCAPE";
    db_ = GetSubsystem<Database>();
    cxn_ = db_->Connect("DSN=" + dsn);

    if (cxn_->IsConnected()) {
        URHO3D_LOGDEBUGF("** SEQUENCER: RECORDER - ODBC Connected! ** -> %s", dsn.CString());
        odbcConnected_ = true;
    }

    schema_ = "world1";

    id_ = "RECORDER-" + String(Random(10000,99999));
    sequence_ = false;

    SubscribeToEvent(E_DBCURSOR, URHO3D_HANDLER(Recorder, HandleDBCursor));
#endif
}

Recorder::~Recorder() {
    if (playSource_)
        playSource_->ReleaseRef();
}

void Recorder::Reset(Context *context) {
    // Restart recording
    data_.Clear();
    // Set store context
    storeContext_ = context;
}

void Recorder::SetPlaySource(SoundSource3D *playSource) {
    playSource_ = playSource;
}

void Recorder::Capture(Beat * channel1_, Beat * channel2_, Beat * channel3_, float currTime_,
                       float beatTime_, float barTime_) {

    BeatTime *t = new BeatTime(currTime_, beatTime_, barTime_);
    // Create new buffer block
    SharedPtr<BufferData> bufData = storeContext_->CreateObject<BufferData>();

    // 1. RECORD SAMPLE INTO BUFFER BLOCK (SHORT STORE)
    bufData->SetData(channel1_, channel2_, channel3_, t);

    data_.Push(SharedPtr(bufData.Get()));

    // This can be pulled from data_ -> bufData

}

int Recorder::GetBufferSize() {
    return data_.Size();
}

bool Recorder::IsODBCConnected() {
    return odbcConnected_;
}

void Recorder::Persist() {
    // 2. RECORD BUFFER BLOCK INTO DATABASE (LONG STORE)

    if (!data_.Empty()) {

        // Write sequence
        int lastUsedSeqId_ = GetSequence();
        URHO3D_LOGDEBUGF("** SEQUENCER: RECORDER - Data Buffer Size ** -> %d, Last Used Seq Id -> %d", GetBufferSize(), lastUsedSeqId_);

        if (!sequence_) {
            // On long store, create sequence
            CreateSequence("seq");
        }


        bool written_ = false;
        for (SharedPtr<BufferData> buf : data_) {


            if (buf.NotNull()) {

                // Only write what has not been written
                if (!buf->IsOnLongStore()) {
                    HashMap<BeatTime *, Vector<BeatTime *>> bufferTime_ = buf->GetTimeData();
                    HashMap<BeatTime *, Vector<Beat *>> bufferBeat_ = buf->GetBeatData();

                    // Vector containing set of channels, for each channel (3)

                    for (BeatTime *t: bufferTime_.Keys()) {
                        // Iterate through each beat time

                        // Retrieve time attributes
                        float currTime_ = t->GetCurrentTime();
                        float beatTime_ = t->GetBeatTime();
                        float barTime_ = t->GetBarTime();

                        Vector<Beat *> vBeat = bufferBeat_.Find(t)->second_;
                        // For time, iterate through each channel beat
/*                for (Beat* b : vBeat) {
                    int sampleIdx = b->GetBeatSampleIdx();
                }*/

                        // Dump buffer to db
                        CreateTimeCode(currTime_, beatTime_);
                        CreatePattern(vBeat[0], vBeat[1], vBeat[2], currTime_, beatTime_, barTime_);
                        written_ = true;
                    }


                    if (written_) {
                        // Mark stored
                        buf->SetOnLongStore(true);
                    }
                }
            }
        }


        // Clear written data buffers
        for (SharedPtr<BufferData> buf : data_) {
            if (buf.NotNull()) {
                if (buf->IsOnLongStore()) {
                    URHO3D_LOGDEBUGF("** SEQUENCER: RECORDER - removing Buffer Data, long store persisted ** -> %s",
                                     buf->GetTypeName().CString());
                    data_.Remove(buf);
                    URHO3D_LOGDEBUGF("** SEQUENCER: RECORDER - Data Buffer Size ** -> %d", GetBufferSize());
    //                buf->ReleaseRef();
                }
            }
        }

    }


}

#ifdef MAYASCAPE_SERVER
void Recorder::HandleDBCursor(StringHash eventType, VariantMap& eventData) {

    using namespace DbCursor;

//    URHO3D_PARAM(P_COLVALUES, ColValues);          // VariantVector
//    URHO3D_PARAM(P_COLHEADERS, ColHeaders);        // StringVector

    VariantVector colValues = eventData[P_COLVALUES].GetVariantVector();
    StringVector colHeaders = eventData[P_COLHEADERS].GetStringVector();

    for (Variant h : colHeaders) {
        String header = h.GetString();
        for (Variant v : colValues) {
            String value = v.GetString();
            URHO3D_LOGDEBUGF("** SEQUENCER: RECORDER - HandleDBCursor value ** -> %s = %s", header.CString(), value.CString());
        }
    }


}
#endif

void Recorder::CreateSequence(String name) {
#ifdef MAYASCAPE_SERVER
    if (!cxn_)
        return;

    seqId_++;
    String seqName = id_ + "-" + name + "-" + String(seqId_);
    String sql = "INSERT INTO " + schema_ + ".ms_sequence (seq_id, seq_name, created) VALUES (" + String("nextval('" + schema_ + ".ms_seq_id')") + ", '" + seqName + "', current_timestamp);";

    URHO3D_LOGDEBUGF("** SEQUENCER: RECORDER - ODBC EXECUTE ** -> %s", sql.CString());
    // Pesist current buffer to long store -> ODBC Postgres
    if (cxn_) {
        if (cxn_->IsConnected()) {
            DbResult result = cxn_->Execute(sql);
            cxn_->Finalize();
        }
    }

    currSeqId_ = GetSequence();
    sequence_ = true;
#endif
}

int Recorder::GetSequence() {
#ifdef MAYASCAPE_SERVER

    if (!cxn_)
        return -1;

    String sql = "SELECT last_value FROM " + schema_ + ".ms_seq_id;";
    //String sql = "SELECT seq_name FROM " + schema_ + ".ms_sequence;";

    URHO3D_LOGDEBUGF("** SEQUENCER: RECORDER - ODBC EXECUTE ** -> %s", sql.CString());
    // Persist current buffer to long store -> ODBC Postgres
    if (cxn_) {
        if (cxn_->IsConnected()) {
            DbResult result = cxn_->Execute(sql);
            VariantVector v = result.GetRows().At(0);

            if (!v.Empty()) {
                //int value = v.At(0).GetInt();
                String val = v.At(0).GetString();
                URHO3D_LOGDEBUGF("** SEQUENCER: RECORDER - ODBC VALUE ** -> %s", val.CString());
                return ToInt(String(val));
            }
        }
    }
#endif
    return 0;
}


void Recorder::CreateTimeCode(float timeCode, float beat) {
#ifdef MAYASCAPE_SERVER
    if (!cxn_)
        return;

    //INSERT INTO world1.ms_time_code (time_code, beat, rec_date) VALUES(0, 0, '');
    String sql = "INSERT INTO " + schema_ + ".ms_time_code (time_code, beat, rec_date) VALUES (" + String( timeCode ) + ", " + String( beat ) + ", current_timestamp);";

    URHO3D_LOGDEBUGF("** SEQUENCER: RECORDER - ODBC EXECUTE ** -> %s", sql.CString());
    // Pesist current buffer to long store -> ODBC Postgres
    if (cxn_) {
        if (cxn_->IsConnected()) {
            cxn_->Execute(sql);
            cxn_->Finalize();
        }
    }
#endif
}

void Recorder::CreatePattern(Beat* channel1_, Beat* channel2_, Beat* channel3_, float currTime_, float beatTime_, float barTime_) {
#ifdef MAYASCAPE_SERVER
    if (!cxn_)
        return;

    String sql;
    sql = "INSERT INTO " + schema_ + ".ms_pattern (seq_id, time_code, channel, sample_idx) VALUES (" + String("currval('" + schema_ + ".ms_seq_id')") + ", " + String(currTime_) + ",0," + String(channel1_->GetBeatSampleIdx()) + ");";
    URHO3D_LOGDEBUGF("** SEQUENCER: RECORDER - ODBC EXECUTE ** -> %s", sql.CString());
    // Pesist current buffer to long store -> ODBC Postgres
    if (cxn_) {
        if (cxn_->IsConnected()) {
            cxn_->Execute(sql);
            cxn_->Finalize();
        }
    }

    sql = "INSERT INTO " + schema_ + ".ms_pattern (seq_id, time_code, channel, sample_idx) VALUES (" + String("currval('" + schema_ + ".ms_seq_id')") + ", " + String(currTime_) + ",1," + String(channel2_->GetBeatSampleIdx()) + ");";
    URHO3D_LOGDEBUGF("** SEQUENCER: RECORDER - ODBC EXECUTE ** -> %s", sql.CString());
    // Pesist current buffer to long store -> ODBC Postgres
    if (cxn_) {
        if (cxn_->IsConnected()) {
            cxn_->Execute(sql);
            cxn_->Finalize();
        }
    }

    sql = "INSERT INTO " + schema_ + ".ms_pattern (seq_id, time_code, channel, sample_idx) VALUES (" + String("currval('" + schema_ + ".ms_seq_id')") + ", " + String(currTime_) + ",2," + String(channel3_->GetBeatSampleIdx()) + ");";
    URHO3D_LOGDEBUGF("** SEQUENCER: RECORDER - ODBC EXECUTE ** -> %s", sql.CString());
    // Pesist current buffer to long store -> ODBC Postgres
    if (cxn_) {
        if (cxn_->IsConnected()) {
            cxn_->Execute(sql);
            cxn_->Finalize();
        }
    }

#endif
}
