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
}

void TLog::Add(UnicodeString ToLog)
{
TJSONObject *json_root;
TJSONArray *json_array;
bool flag=0;
if (!this->Output->Parent->Visible) {
	this->Output->Parent->Show();
	}
unsigned int i,j=1;//split complex JSON string into simple
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
	for (i = 0; i < ToLogArr.size(); i++) {//and show each of them
		flag=false;
		json_root = (TJSONObject*) TJSONObject::ParseJSONValue(TEncoding::ASCII->GetBytes(ToLogArr.operator [](i)),0);
		if ((json_root)&&(json_root->Get("id"))) {
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
				if ((json_root->Get("result"))&&(json_root->Get("result")->JsonValue->ToString()=="true")) {
					this->Body->operator =("Share accepted");
					flag=true;
					}
				}//mining.submit END
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
