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
	this->LogWnd->SelAttributes->Color = this->TextColor;
	if ((this->LogWnd->Lines->Count > 0)&&(this->LogWnd->Lines->operator [](this->LogWnd->Lines->Count - 1) != "")) {
		this->LogWnd->Lines->Add("");
		}
	this->LogWnd->Lines->Add(DateTimeToStr(Now())+" "+this->Header);
	if (!Debugstring1->Checked) {
		this->LogWnd->Lines->Add(RemoveLowChar(ToLog));
		//this->LogWnd->Lines->Add(ToLog);
		} else {
			//this->LogWnd->Lines->AddStrings(DebugStr(ToLog,*Strings));
			//this->LogWnd->Lines->AddStrings(Strings);
			}
	this->LogWnd->SelStart = OrgPos;
	Perform(EM_SCROLLCARET,0,0);
	this->LogWnd->Lines->EndUpdate();
	Strings->Free();
	} catch (...) {
		this->LogWnd->Lines->EndUpdate();
		Strings->Free();
		}
	if (this->Refresh1->Checked) {
		this->LogWnd->SelStart = this->LogWnd->GetTextLen();
		Perform(EM_SCROLLCARET,0,0);
		}
}
void __fastcall TLog::FormCreate(TObject *Sender)
{
this->LogWnd->Clear();
this->LogWnd->Align = alClient;
this->Header = "";
this->TextColor = clBlack;
}
//---------------------------------------------------------------------------
UnicodeString RemoveLowChar(UnicodeString Source)
{
int n;
UnicodeString result;
result.SetLength(Source.Length());
for (n = 1; n < Source.Length()+1; n++) {
	result[n] = _DebugStrLowChar(Source[n]);
	}
return result;
}

char _DebugStrLowChar(char c)
{
if (((int)c)<32) {
	c = '.';
	}
return c;
/*
__asm{
	cmp al, ' '
	jae @Ok
	mov al, '.'
	@Ok:
	}
return c;*/
}

