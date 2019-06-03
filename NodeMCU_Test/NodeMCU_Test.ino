#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <string.h>
#include <ros.h>
#include <aris_msgs/aris_service.h>
#include <aris_msgs/aris_service.h>
#include "NodeMCUClass.h"

#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
#define WIFI_SSID "AndroidAP"
#define WIFI_PASSWORD ""

ros::NodeHandle  nh;
using aris_msgs::aris_service;

Handler Object;
    int source;
    int destination;
    int material;
    int fetched;
    int status1;
    int prev_req_no=-1;
    int current_req=-1;
        
//callback function called from subscriber 
void callback (const aris_service::Request & req, aris_service::Response & res){
  
    String TotalCount     = String("ARIS_Requests_Summary/ARIS_Total_Count");
    String PendingPath    = String("ARIS_Requests_Summary/ARIS_Pending_Count");

    int Req_No=1;

  
    float Temp1 = Firebase.getFloat(TotalCount);
    int Total=(int) Temp1;
    float pending = Firebase.getFloat(PendingPath);
    int pendingno=(int) pending;


    JumpHere :
    
if(pendingno>=0){
    
    Serial.println(Req_No);

    fetched = Object.getFetched(Req_No);
    Serial.println(fetched);
      
    status1 = Object.getstatus(Req_No);
    Serial.println(status1);

  if((fetched==0 && status1==0) && ((req.aris_req_status==0) || (req.aris_req_status==1)) ){
    
    source = Object.getSource(Req_No);
    Serial.println(source);
    
    destination = Object.getDestination(Req_No);
    Serial.println(destination);
    
    material = Object.getMaterial(Req_No);
    Serial.println(material);

    Serial.println("---------------------------");

    res.aris_material     = material;
    res.aris_source       = source;
    res.aris_destination  = destination;

    Object.setFetched(Req_No);
      if(pendingno!=0){
      pendingno--;
      Object.setPending(pendingno);
      prev_req_no=Req_No;
      current_req=Req_No;
      }
      }
  else if(fetched==1 && req.aris_req_status==0){
    Req_No++;
    goto JumpHere;
  }
  else if(fetched==1 && status1==0 && req.aris_req_status==1 && (current_req==Req_No)){
    Object.setstatus(current_req);
    Req_No++;
    Serial.println(Req_No);
    if(pendingno!=0){
       goto JumpHere; 
    }  
  }
  else if(fetched==1 && req.aris_req_status==1){
    Req_No++;
    goto JumpHere;
  }
  else{
    Serial.print("ERROR");
  }
}else{
    res.aris_material     = -1;
    res.aris_source       = -1;
    res.aris_destination  = -1;
}
}

ros::ServiceServer<aris_service::Request, aris_service::Response> server("aris_firebase",&callback);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  nh.initNode();
  nh.advertiseService(server);
}

void loop() {

  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }

  nh.spinOnce();
  delay(100);
}
