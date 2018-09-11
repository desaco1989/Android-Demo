#include <binder/IServiceManager.h>
#include <binder/IBinder.h>
#include <binder/Parcel.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <private/binder/binder_module.h>
 
using namespace android;
#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "sampleCallback"
#define SAMPLE_SERIVCE_DES "sample.hello"
#define SAMPLE_CB_SERIVCE_DES "android.os.SampleCallback"
#define SRV_CODE 1
#define CB_CODE 1
class SampeCallback : public BBinder
{
public:
  SampeCallback()
  {
    mydescriptor = String16(SAMPLE_CB_SERIVCE_DES);
  }
  virtual ~SampeCallback() {
          
  }
  virtual const String16& getInterfaceDescriptor() const{
    return mydescriptor;
  }
protected:
     
  void callbackFunction() {
    ALOGI( "Client callback function called-----------cb" );
  }
     
  virtual status_t onTransact( uint32_t code,
			       const Parcel& data,Parcel* reply,uint32_t flags = 0){
    ALOGD( "Client onTransact, code = %d", code);   
         
    switch (code){
    case CB_CODE:
      callbackFunction();
      break;
    default:
      return BBinder::onTransact(code, data, reply, flags);
    }
    return 0;
  }
private:
  String16 mydescriptor;
};

int main()
{
  sp<IServiceManager> sm = defaultServiceManager();
  sp<IBinder> ibinder = sm->getService(String16(SAMPLE_SERIVCE_DES));
  if (ibinder == NULL){
    ALOGW( "Client can't find Service" );
           return -1;
     }
     Parcel _data,_reply;
     SampeCallback *callback = new SampeCallback();
     //注册回调函数
     _data.writeStrongBinder(sp<IBinder>(callback));
     int ret = ibinder->transact(SRV_CODE, _data, &_reply, 0);

//   ProcessState::self()->startThreadPool();
//   IPCThreadState::self()->joinThreadPool();
     while(1);
     return 0;
}
