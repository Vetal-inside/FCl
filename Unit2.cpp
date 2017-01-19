//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
#include <DBXJSON.hpp>
#include <vector>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormCreate(TObject *Sender)
{
this->LogWnd->Clear();
this->LogWnd->Align = alClient;
Log = new TLog(this->LogWnd);
}
//---------------------------------------------------------------------------
TLog::TLog(TRichEdit* RE)
{
this->Header = new UnicodeString;
this->Body = new UnicodeString;
this->Output = RE;
this->LogLevel = 1;
this->LogVersion = 0;
}

void TLog::Add(UnicodeString ToLog)
{
TJSONObject *json_root, *json_subroot;
TJSONArray *json_array;
bool flag=0;
if (!this->Output->Parent->Visible) {
	this->Output->Parent->Show();
	}
int i,j=1;//split complex JSON string into simple
std::vector<UnicodeString> ToLogArr;
ToLogArr.resize(0);
for (i = 1; i < ToLog.Length()+1; i++) {
	if (ToLog[i]=='\n') {
		ToLogArr.resize(ToLogArr.size()+1);
		ToLogArr.operator [](ToLogArr.size()-1).operator =(ToLog.SubString(j,i-j));
		j=i+1;
		}
	}
ToLogArr.resize(ToLogArr.size()+1);
ToLogArr.operator [](ToLogArr.size()-1).operator =(ToLog.SubString(j,i-j));
__try {
	this->Output->Lines->Add(DateTimeToStr(Now())+" "+*this->Header);
	for (i = 0; i < (int)ToLogArr.size(); i++) {//and show each of them
		flag=false;
		json_root = (TJSONObject*) TJSONObject::ParseJSONValue(TEncoding::ASCII->GetBytes(ToLogArr.operator [](i)),0);
		if (((json_root)&&(json_root->Get("id")))&&(this->LogLevel < 2)) {//if loglevel == 2, that we show full packet data
			switch (this->LogVersion) {
				case 0://cm91z
				case 1://cm93z_pl
					if (json_root->Get("id")->JsonValue->ToString() == "1") {//mining.subscribe BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"mining.subscribe\"")) {
							json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
							this->Body->operator =("Connecting to pool: "+json_array->Get(2)->ToString()+":"+json_array->Get(3)->ToString());
							flag=true;
							}
						if ((json_root->Get("result"))&&(json_root->Get("error")->JsonValue->ToString()=="null")) {
							json_array = (TJSONArray*) json_root->Get("result")->JsonValue;
							this->Body->operator =("Succesfully connected! Params: "+json_array->Get(0)->ToString()+", "+json_array->Get(1)->ToString());
							flag=true;
							}
						}//mining.subscribe END
					if (json_root->Get("id")->JsonValue->ToString() == "2") {//mining.authorize BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"mining.authorize\"")) {
							json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
							this->Body->operator =("Autorizing : "+json_array->Get(0)->ToString()+":"+json_array->Get(1)->ToString());
							flag=true;
							}
						if ((json_root->Get("result"))&&(json_root->Get("result")->JsonValue->ToString()=="true")) {
							this->Body->operator =("Succesfully authorized!");
							flag=true;
							}
						}//mining.authorize END
					if (json_root->Get("id")->JsonValue->ToString() == "4") {//mining.submit BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"mining.submit\"")) {
							json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
							this->Body->operator =("Sending share from : "+json_array->Get(0)->ToString());
							flag=true;
							}
						if (json_root->Get("result")) {
							if (json_root->Get("result")->JsonValue->ToString()=="true") {
								this->Body->operator =("Share accepted");
								flag=true;
								}
							if (json_root->Get("result")->JsonValue->ToString()=="null") {
								json_array = (TJSONArray*) json_root->Get("error")->JsonValue;
								this->Body->operator =("Share rejected. Reason: "+json_array->Get(1)->ToString()+", error id "+json_array->Get(0)->ToString());
								flag=true;
								}
							}
						}//mining.submit END
					if (json_root->Get("id")->JsonValue->ToString() == "5") {//mining.extranonce.subscribe BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"mining.extranonce.subscribe\"")) {
							this->Body->operator =("Requesting extranonce.subscribe...");
							flag=true;
							}
						if (json_root->Get("result")) {
							if (json_root->Get("result")->JsonValue->ToString()=="true") {
								this->Body->operator =("Extranonce.subscribe succesful!");
								flag=true;
								}
							}
						}//mining.extranonce.subscribe END
					if (json_root->Get("id")->JsonValue->ToString() == "null") {//mining.set_target/notify/set_extranonce BEGIN
						if (json_root->Get("method")) {
							if (json_root->Get("method")->JsonValue->ToString() == "\"mining.set_target\""){
								json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
								this->Body->operator =("Pool set new target with params: "+json_array->Get(0)->ToString());
								flag=true;
								}
							if (json_root->Get("method")->JsonValue->ToString() == "\"mining.set_extranonce\""){
								json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
								this->Body->operator =("Pool set new extranonce with params: "+json_array->Get(0)->ToString());
								flag=true;
								}
							if (json_root->Get("method")->JsonValue->ToString() == "\"mining.notify\""){
								json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
								this->Body->operator =("mining.notify with params: ");
								for (j = 0; j < json_array->Size(); j++) {
									this->Body->operator +=(json_array->Get(j)->ToString()+", ");
									}
								flag=true;
								}
							}
						}//mining.set_target/notify/set_extranonce BEGIN
					break;
				case 2://cm74et
					if (json_root->Get("id")->JsonValue->ToString() == "2") {//eth_submitLogin BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"eth_submitLogin\"")) {
							json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
							this->Body->operator =("Autorizing : "+json_array->Get(0)->ToString()+":"+json_array->Get(1)->ToString());
							flag=true;
							}
						if ((json_root->Get("result"))&&(json_root->Get("result")->JsonValue->ToString()=="true")) {
							this->Body->operator =("Succesfully authorized!");
							flag=true;
							}
						}//eth_submitLogin END
					if (json_root->Get("id")->JsonValue->ToString() == "3") {//eth_getWork BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"eth_getWork\"")) {
							this->Body->operator =("Getting new work...");
							flag=true;
							}
						if (json_root->Get("result")) {
							json_array = (TJSONArray*) json_root->Get("result")->JsonValue;
							this->Body->operator =("Pools send new work: " + json_array->Get(0)->ToString()+", "+json_array->Get(1)->ToString()+", "+json_array->Get(2)->ToString());
							flag=true;
							}
						}//eth_getWork END
					if (json_root->Get("id")->JsonValue->ToString() == "4") {//eth_submitWork BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"eth_submitWork\"")) {
							this->Body->operator =("Sending share");
							flag=true;
							}
						if (json_root->Get("result")) {
							if (json_root->Get("result")->JsonValue->ToString()=="true") {
								this->Body->operator =("Share accepted");
								flag=true;
								}
							}
						}//eth_submitWork END
					if (json_root->Get("id")->JsonValue->ToString() == "6") {//eth_submitHashrate BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"eth_submitHashrate\"")) {
							this->Body->operator =("Sending hashrate...");
							flag=true;
							}
						if (json_root->Get("result")) {
							if (json_root->Get("result")->JsonValue->ToString()=="true") {
								this->Body->operator =("Hashrate accepted");
								flag=true;
								}
							}
						}//eth_submitHashrate END
					break;
				case 3://cm97x
					if (json_root->Get("id")->JsonValue->ToString() == "1") {//login BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"login\"")) {
							json_subroot = (TJSONObject*) json_root->Get("params")->JsonValue;
							this->Body->operator =("Login to pool from: "+json_subroot->Get("login")->JsonValue->ToString()+":"+json_subroot->Get("pass")->JsonValue->ToString());
							flag=true;
							}
						if ((json_root->Get("result"))&&(json_root->Get("error")->JsonValue->ToString()=="null")) {
							json_subroot = (TJSONObject*) json_root->Get("result")->JsonValue;
							if (json_subroot->Get("status")->JsonValue->ToString() == "\"OK\"") {
								this->Body->operator =("Succesfully connected!");
								flag=true;
								}
							}
						}//login END
					if (json_root->Get("id")->JsonValue->ToString() == "4") {//submit BEGIN
						if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"submit\"")) {
							this->Body->operator =("Sending share");
							flag=true;
							};
						if (json_root->Get("result")) {
							json_subroot = (TJSONObject*) json_root->Get("result")->JsonValue;
							if (json_subroot&&json_subroot->Get("status")) {
								if (json_subroot->Get("status")->JsonValue->ToString() == "\"OK\"") {
									this->Body->operator =("Share accepted");
									flag=true;
									}
								}
							}
						}//submit END
					break;
				}
			};
		if (!flag) {
			this->Body->operator =(ToLogArr.operator [](i));
			}
		this->Output->Lines->Add(*this->Body);
		};
	}__finally {
		if (json_root) {json_root->Free();};
		};
}

void TLog::RemoveLowChar()
{
for (int n = 1; n < this->Body->Length()+1; n++) {
	if (((int)this->Body->operator [](n))<32) {
		this->Body->operator [](n) = '.';
		};
	}
return ;
}

TLog::~TLog()
{

}
