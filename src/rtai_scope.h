#ifndef RTAI_SCOPE_H
#define RTAI_SCOPE_H


#include "target_thread.h"
class RtaiScopeThread : public QThread
 {
// Q_OBJECT
 public:
//                                                RtaiScopeThread();
//                                                ~RtaiScopeThread();
//                void 				kill();
//                bool 				isRunning();
//                bool				isInitialized();

        protected:
                //reimplement virtual run() from QThread
                void 				run();

private:
        RT_TASK *GetScopeDataTask;
        MBX *GetScopeDataMbx;
        char GetScopeDataMbxName[7];
        long GetScopeDataPort;
        int MsgData, MsgLen, MaxMsgLen, TracesBytes,MsgWait;
        float MsgBuf[MAX_MSG_LEN/sizeof(float)];
        int n, nn, js, jl;
        int index;
        char *mbx_id ;
        int save_idx;
        TargetThread* targetThread;
        int hardRealTime;
        QPL_ScopeData* scope;
        double dt;
         long Target_Node ;
        RT_TASK *Target_Interface_Task;
 };





#endif // RTAI_SCOPE_H
