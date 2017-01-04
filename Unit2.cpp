//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
#include <DBXJSON.hpp>
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

void TLog::Add(UnicodeString ToLog, bool IsPretty)
{
if (!IsPretty) {
	this->Body->operator =(ToLog);
	}
RemoveLowChar();
int OrgPos;
TStringList* Strings = new TStringList();
OrgPos = this->Output->SelStart;
if (!this->Output->Parent->Visible) {
	this->Output->Parent->Show();
	}
this->Output->Lines->BeginUpdate();
try {
	this->Output->SelStart = this->Output->GetTextLen();
	if ((this->Output->Lines->Count > 0)&&(this->Output->Lines->operator [](this->Output->Lines->Count - 1) != "")) {
		this->Output->Lines->Add("");
		}
	this->Output->Lines->Add(DateTimeToStr(Now())+" "+*this->Header);
	this->Output->Lines->Add(*this->Body);
	this->Output->SelStart = OrgPos;
	this->Output->Parent->Perform(EM_SCROLLCARET,0,0);
	this->Output->Lines->EndUpdate();
	Strings->Free();
	} catch (...) {
		this->Output->Lines->EndUpdate();
		Strings->Free();
		}
this->Output->SelStart = this->Output->GetTextLen();
this->Output->Parent->Perform(EM_SCROLLCARET,0,0);
}

void TLog::AddPrettyView(UnicodeString& ToLog)
{
TJSONObject *json_root;
TJSONArray *json_array;
this->Body->operator =(ToLog);
__try {
	json_root = (TJSONObject*) TJSONObject::ParseJSONValue(TEncoding::ASCII->GetBytes(*this->Body),0);
	if ((json_root)&&(json_root->Get("id"))) {
		if (json_root->Get("id")->JsonValue->ToString() == "1") {//mining.subscribe BEGIN
			if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"mining.subscribe\"")) {
				json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
				this->Body->operator =("Connecting to pool: "+json_array->Get(2)->ToString()+":"+json_array->Get(3)->ToString());
				this->Add(*this->Body,true);
				}
			if ((json_root->Get("result"))&&(json_root->Get("error")->JsonValue->ToString()=="null")) {
				json_array = (TJSONArray*) json_root->Get("result")->JsonValue;
				this->Body->operator =("Succesfully connected! Params: "+json_array->Get(0)->ToString()+", "+json_array->Get(1)->ToString());
				this->Add(*this->Body,true);
				}
			}//mining.subscribe END
		if (json_root->Get("id")->JsonValue->ToString() == "2") {//mining.authorize BEGIN
			if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"mining.authorize\"")) {
				json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
				this->Body->operator =("Autorizing : "+json_array->Get(0)->ToString()+":"+json_array->Get(1)->ToString());
				this->Add(*this->Body,true);
				}
			if ((json_root->Get("result"))&&(json_root->Get("result")->JsonValue->ToString()=="true")) {
				this->Body->operator =("Succesfully authorized!");
				this->Add(*this->Body,true);
				}
			}//mining.authorize END
		if (json_root->Get("id")->JsonValue->ToString() == "4") {//mining.submit BEGIN
			if (json_root->Get("method")&&(json_root->Get("method")->JsonValue->ToString() == "\"mining.submit\"")) {
				json_array = (TJSONArray*) json_root->Get("params")->JsonValue;
				this->Body->operator =("Sending share from : "+json_array->Get(0)->ToString());
				this->Add(*this->Body,true);
				}
			if ((json_root->Get("result"))&&(json_root->Get("result")->JsonValue->ToString()=="true")) {
				this->Body->operator =("Share accepted");
				this->Add(*this->Body,true);
				}
			}//mining.submit END
		} else {
			this->Add(ToLog,false);
			}
	}
	__finally {
		json_root->Free();
		}
return ;
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
