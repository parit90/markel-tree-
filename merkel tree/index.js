let fs = require('fs');
const addon = require('./build/Release/MerkelTree');
let MAP = new Map();
let MapArray = new Array();

function MemoizationDirFile(cb){
    fs.readdir('./files',function(err,files){
        let filesLenght = files.length;
        if(err){
            console.log(err)
        }
        else{
            files.forEach(function(fileName){
                fs.readFile('./files/'+fileName,'utf-8',function(err,content){
                    if(err){
                        console.log("err...",err);
                    }
                    else{
                        let tempObj = new Object();
                        let strArr = new String();
                        strArr = fileName.split(".")[0];
                        content = JSON.parse(content);
                        tempObj["key"] = strArr;
                        tempObj["value"] = content;
                        MapArray.push(tempObj);
                        if(MapArray.length === filesLenght) cb(null,MapArray);
                    }
                })
            })
            
        }
    })
};

 MemoizationDirFile(function(err,result){
     if(err){
         console.log(err)
     }
     else{
         const obj = {};
        //  Object.defineProperty(obj,'MapData',{
        //     value: result,
        //     writable: false
        //  })
         obj["MapData"] = result;
         rootHash = addon.MerkelTree(obj);
         console.log("addon ...",rootHash);
     }
 });
    
