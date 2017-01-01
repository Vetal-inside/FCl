//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uLog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLog *Log;
//---------------------------------------------------------------------------
__fastcall TLog::TLog(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TLog::Add(UnicodeString ToLog)
{
int OrgPos;
TStringList* Strings = new TStringList();
if (!this->Visible) { this->Show();};
OrgPos = this->LogWnd->SelStart;
this->LogWnd->Lines->BeginUpdate();
try {
	this->LogWnd->SelStart = this->LogWnd->GetTextLen();
	if ((this->LogWnd->Lines->Count > 0)&&(this->LogWnd->Lines->operator [](this->LogWnd->Lines->Count - 1) != "")) {
		this->LogWnd->Lines->Add("");
		}
	this->LogWnd->Lines->Add(DateTimeToStr(Now())+" "+this->Header);
	this->LogWnd->Lines->Add(RemoveLowChar(ToLog));
	this->LogWnd->SelStart = OrgPos;
	Perform(EM_SCROLLCARET,0,0);
	this->LogWnd->Lines->EndUpdate();
	Strings->Free();
	} catch (...) {
		this->LogWnd->Lines->EndUpdate();
		Strings->Free();
		}
	this->LogWnd->SelStart = this->LogWnd->GetTextLen();
	Perform(EM_SCROLLCARET,0,0);
}
void __fastcall TLog::FormCreate(TObject *Sender)
{
this->LogWnd->Clear();
this->LogWnd->Align = alClient;
this->Header = "";
}
//---------------------------------------------------------------------------
UnicodeString& TLog::RemoveLowChar(UnicodeString& Source)
{
int n;
UnicodeString* Result = new UnicodeString;
Result->SetLength(Source.Length());
for (n = 1; n < Source.Length()+1; n++) {
	if (((int)Source[n])<32) {
		Result[n] = '.';
		} else {
			Result[n] = Source[n];
			}
	}
return *Result;
}
