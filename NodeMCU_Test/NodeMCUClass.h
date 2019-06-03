class Handler {

public:

 int Source =0;
 int Destination =0;
 int Material =0;
 int Fetched =-1;
 int Status =-1;

 Handler();
 void Print(int Req_No);
 int getSource(int Req_No);
 int getDestination(int Req_No);
 int getMaterial(int Req_No);
 int getFetched(int Req_No);
 int getstatus(int Req_No); 
 bool setFetched(int Req_No);
 bool setstatus(int Req_No);
 bool setPending(int pendingno);
};

Handler::Handler()
{
}

int Handler::getSource(int Req_No)
{
    String Aris_Node1 = "ARIS_User_Requests/ARIS_Request_";
    String RequestPath    = Aris_Node1+Req_No;
    String SourcePath     = String(RequestPath+"/"+"aris_source");
    Source = Firebase.getInt(SourcePath);
    return Source;
}

int Handler::getDestination(int Req_No)
{
    String Aris_Node1 = "ARIS_User_Requests/ARIS_Request_";
    String RequestPath    = Aris_Node1+Req_No;
    String DestinationPath= String(RequestPath+"/"+"aris_destination");
    Destination = Firebase.getInt(DestinationPath);
    return Destination;
}

int Handler::getMaterial(int Req_No)
{
    String Aris_Node1 = "ARIS_User_Requests/ARIS_Request_";
    String RequestPath    = Aris_Node1+Req_No;
    String MaterialPath   = String(RequestPath+"/"+"aris_material");
    Material = Firebase.getInt(MaterialPath);
    return Material;
}

int Handler::getFetched(int Req_No)
{
    String Aris_Node1 = "ARIS_User_Requests/ARIS_Request_";
    String RequestPath    = Aris_Node1+Req_No;
    String FetchedPath    = String(RequestPath+"/"+"aris_req_fetched");
    Fetched = Firebase.getInt(FetchedPath);
    return Fetched;
}

bool Handler::setFetched(int Req_No)
{
    String Aris_Node1 = "ARIS_User_Requests/ARIS_Request_";
    String RequestPath    = Aris_Node1+Req_No;
    String FetchedPath    = String(RequestPath+"/"+"aris_req_fetched");
    Firebase.setInt(FetchedPath,1);
    return true;
}

int Handler::getstatus(int Req_No)
{
    String Aris_Node1 = "ARIS_User_Requests/ARIS_Request_";
    String RequestPath    = Aris_Node1+Req_No;
    String StatusPath     = String(RequestPath+"/"+"aris_req_status");
    Status = Firebase.getInt(StatusPath);
    return Status;
}

bool Handler::setstatus(int Req_No)
{
    String Aris_Node1 = "ARIS_User_Requests/ARIS_Request_";
    String RequestPath    = Aris_Node1+Req_No;
    String StatusPath     = String(RequestPath+"/"+"aris_req_status");
    Firebase.setInt(StatusPath,1);
    return true;
}

bool Handler::setPending(int pendingno)
{
    String PendingPath     = String("ARIS_Requests_Summary/ARIS_Pending_Count");
    Firebase.setInt(PendingPath,pendingno);
    return true;
}
















void Handler::Print(int Req_No)
{
    String Aris_Node1 = "ARIS_Request_";
    String RequestPath    = Aris_Node1+Req_No;

    String SourcePath     = String(RequestPath+"/"+"aris_source");
    String DestinationPath= String(RequestPath+"/"+"aris_destination");
    String MaterialPath   = String(RequestPath+"/"+"aris_material");
    String FetchedPath    = String(RequestPath+"/"+"aris_req_fetched");
    String StatusPath     = String(RequestPath+"/"+"aris_req_status");


    // get value 
    Serial.print("Request Number : ");
    Serial.println(Req_No);    

    Serial.print("source: ");
    Source = Firebase.getInt(SourcePath);
    Serial.println(Source);
    delay(1000);

    Serial.print("Destination: ");
    Destination = Firebase.getInt(DestinationPath);
    Serial.println(Destination);
    delay(1000);

    Serial.print("Material: ");
    Material = Firebase.getInt(MaterialPath);
    Serial.println(Material);
    delay(1000);

    Serial.print("Fetched : ");
    Fetched = Firebase.getInt(FetchedPath);
    Serial.println(Fetched);
    delay(1000);

    Serial.print("Req. Completed : ");
    Status = Firebase.getInt(StatusPath);
    Serial.println(Status);
    delay(1000);

    Serial.println("___________________");
  
}
