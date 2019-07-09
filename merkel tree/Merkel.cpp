#include <node.h>
#include <vector>
#include <deque>
#include <math.h>
#include "Merkel.h"
#include "sha256.h" // using existed lib. pre-built header  
#include "sha256.cpp" // using existed cpp. for creating hash of the data

using namespace v8;
using namespace std;

vector<string> _hashes;
string CreateDataToHash(string strr,const FunctionCallbackInfo<Value>& args);
string createMerkelRoot(deque<string> _hashes, int height);
string createHash(Isolate *isolate,const FunctionCallbackInfo<Value>& args,const Handle<Object> dataToHash);

void Main(const FunctionCallbackInfo<Value>& args){
    deque<string> _hashes;
    Isolate* isolate = args.GetIsolate();
    Handle<Object> InputData = Handle<Object>::Cast(args[0]);
    
    Handle<Array> array = Handle<Array>::Cast(InputData ->Get(String::NewFromUtf8(isolate,"MapData")));
    
    for(int i=0;i<array->Length();i++){
         string _s = createHash(isolate, args, Handle<Object>::Cast(array->Get(i)));
        _hashes.push_back(_s); 
    }
    int height = ceil(log2(_hashes.size()));
    string _rootHash = createMerkelRoot(_hashes,height);
    // for(vector<string>::iterator it = _hashes.begin();it!=_hashes.end();it++){
    //     cout<<"it ="<<*it<<endl;
    // }
    //string _merkelRoot = createMerkelRoot(_hashes);
    //args.GetReturnValue().Set(array);
}

string createHash(Isolate *isolate,const FunctionCallbackInfo<Value>& args,Handle<Object> dataToHash) {
    Handle<Object> _toHash = Handle<Object>::Cast(dataToHash ->Get(String::NewFromUtf8(isolate,"value")));  
    Handle<Value> nameParam = _toHash ->Get(String::NewFromUtf8(isolate,"name"));
    Handle<Value>  isActiveParam = _toHash ->Get(String::NewFromUtf8(isolate,"active"));
    bool val = isActiveParam->BooleanValue();
    v8::String::Utf8Value str(nameParam);
    std::string strr(*str);
    Local<String> _str = String::NewFromUtf8(isolate,strr.c_str());
    string __str = CreateDataToHash(strr,args); 
    //args.GetReturnValue().Set(val);
    return __str;
}

string CreateDataToHash(string strr,const FunctionCallbackInfo<Value>& args){
    SHA256 sha256;
    //args.GetReturnValue().Set(strr);
    return sha256(strr);
}

string createMerkelRoot(deque<string> _hashes,int height){
    cout<<"---createMerkelRoot---"<<height<<"==="<<_hashes.size()<<endl;
    /**
     * Height of tree:
     * height of tree is depend on number of leaf node
     * if we have (n) leaf then we can have tree having height atleast of log(n)
     * eg. 8 leaf node having height of log(8) with base 2.
    */
    SHA256 sha256;
    while(height!=0){
        int i=0,j=1;
        //vector<string> _nodeHash[pow(2,height)];
        while(j!=pow(2,height)){
            string _tempStr = sha256(_hashes[i]+_hashes[j]);
            cout<<"_tempStr = "<<_tempStr<<endl;
            _hashes.erase(_hashes.begin()+i ,_hashes.begin()+j);
            _hashes.push_front(_tempStr);
            i = i+2,j = j+2;
         }
         height = height-1;
    }
    cout<<"_hashes"<<_hashes[0]<<endl;
    return "Root";
}


void init(Local<Object> exports){
    NODE_SET_METHOD(exports,"MerkelTree",Main);
}
NODE_MODULE(MerkelTree,init);
