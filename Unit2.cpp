//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
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
	this->Output->Lines->Add(RemoveLowChar(ToLog));
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

UnicodeString& TLog::RemoveLowChar(UnicodeString& Source)
{
this->Body->operator =(Source);

for (int n = 1; n < Source.Length()+1; n++) {
	if (((int)Source[n])<32) {
		this->Body->operator [](n) = '.';
		};
	}
return *this->Body;
}

TLog::~TLog()
{

}
