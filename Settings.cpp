//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Settings.h"

#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm3::FormShow(TObject *Sender)
{
this->LoadNetworkSettings();
this->FillAdaptersData();
this->FillHostsData();
this->FillNetworkData();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::LoadNetworkSettings()
{
/* Some general variables */
ULONG ulOutBufLen;
DWORD dwRetVal;
unsigned int i, j = 0;

/* variables used for GetNetworkParams */
FIXED_INFO *pFixedInfo;
IP_ADDR_STRING *pIPAddr;
/* variables used for GetAdapterInfo */
IP_ADAPTER_INFO *pAdapterInfo;
IP_ADAPTER_INFO *pAdapter;

ULONG NTEInstance;

//This is GetAdaptersInfo
pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof (IP_ADAPTER_INFO));
ulOutBufLen = sizeof (IP_ADAPTER_INFO);

if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
	FREE(pAdapterInfo);
	pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
	};

if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) != NO_ERROR) {
	//GetAdaptersInfo failed with error
	if (pAdapterInfo) {FREE(pAdapterInfo);};
	}else {
		pAdapter = pAdapterInfo;
		while (pAdapter) {
			j++;
			NetworkConfigs->resize(j);
			NetworkConfigs->operator [](j-1).AdapterName = pAdapter->AdapterName;
			NetworkConfigs->operator [](j-1).AdapterDescription = pAdapter->Description;
			NetworkConfigs->operator [](j-1).ipAddress = pAdapter->IpAddressList.IpAddress.String;//!!!!!LISST!!!!!
			NetworkConfigs->operator [](j-1).Mask = pAdapter->IpAddressList.IpMask.String;//!!!!!LISST!!!!!
			while (pAdapter->IpAddressList.Next) {
				pAdapter->IpAddressList = *pAdapter->IpAddressList.Next;
				if (pAdapter->IpAddressList.IpAddress.String[0] != '8') {
					NetworkConfigs->operator [](j-1).ipAddress = pAdapter->IpAddressList.IpAddress.String;//!!!!!LISST!!!!!
					NetworkConfigs->operator [](j-1).Mask = pAdapter->IpAddressList.IpMask.String;//!!!!!LISST!!!!!
					break;
					};
				};
			NetworkConfigs->operator [](j-1).Gateway = pAdapter->GatewayList.IpAddress.String;//!!!!!LISST!!!!!
			pAdapter = pAdapter->Next;
			};
		};

j = 0;
//This is GetNetworkParams
pFixedInfo = (FIXED_INFO *) MALLOC(sizeof (FIXED_INFO));
ulOutBufLen = sizeof (FIXED_INFO);
if (GetNetworkParams(pFixedInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
	FREE(pFixedInfo);
	pFixedInfo = (FIXED_INFO *) MALLOC(ulOutBufLen);
	};

if ((dwRetVal = GetNetworkParams(pFixedInfo, &ulOutBufLen)) != NO_ERROR) {
	//GetNetworkParams failed with error
	if (pFixedInfo) {FREE(pFixedInfo);};
		} else {
			j++;
			NetworkConfigs->operator [](0).DNS.resize(j);
			NetworkConfigs->operator [](0).DNS[j-1] = pFixedInfo->DnsServerList.IpAddress.String;
			pIPAddr = pFixedInfo->DnsServerList.Next;
			while (pIPAddr) {
				j++;
				NetworkConfigs->operator [](0).DNS.resize(j);
				NetworkConfigs->operator [](0).DNS[j-1] = pIPAddr->IpAddress.String;
				pIPAddr = pIPAddr->Next;
				}
			};

/* Free allocated memory no longer needed */
if (pFixedInfo) {
	FREE(pFixedInfo);
	pFixedInfo = NULL;
	};
if (pAdapterInfo) {
	FREE(pAdapterInfo);
	pAdapterInfo = NULL;
	};



// Set the flags to pass to GetAdaptersAddresses
ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
// default to unspecified address family (both)
ULONG family = AF_INET;
PIP_ADAPTER_ADDRESSES pAddresses = NULL;
ULONG outBufLen = 0;
ULONG Iterations = 0;
PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
// Allocate a 15 KB buffer to start with.
outBufLen = WORKING_BUFFER_SIZE;

do {
	pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
	if (pAddresses == NULL) {
		//Memory allocation failed for IP_ADAPTER_ADDRESSES struct
		exit(1);
		}
	dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

	if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
		FREE(pAddresses);
		pAddresses = NULL;
		} else {
			break;
			};
	Iterations++;
} while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < 3));

j=0;
i=0;
if (dwRetVal == NO_ERROR) {
	// If successful, output some information from the data we received
	pCurrAddresses = pAddresses;
	while (pCurrAddresses) {
		for (i = 0; i < NetworkConfigs->size(); i++) {
			if (NetworkConfigs->operator [](i).AdapterName == pCurrAddresses->AdapterName) {
				NetworkConfigs->operator [](j).FriendlyName = pCurrAddresses->FriendlyName;
				break;
				}
			}
		j++;
		pCurrAddresses = pCurrAddresses->Next;
		};
	};

if (pAddresses) {
	FREE(pAddresses);
	};
}
//---------------------------------------------------------------------------
void __fastcall TForm3::FillAdaptersData()
{
unsigned int i;
this->ComboBox4->Items->Clear();
for (i = 0; i < NetworkConfigs->size(); i++) {
	this->ComboBox4->Items->Add(NetworkConfigs->operator [](i).FriendlyName);
	}
this->ComboBox4->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::FillHostsData()
{
this->Memo1->Lines->Clear();
int i;
int rig_number = this->ComboBox1->ItemIndex + 1;
for (i = 0; i < Logic->GetPoolsCount(); i++) {
	this->Memo1->Lines->Add("8.8." + IntToStr(rig_number) + ".10"+IntToStr(i)+"  "+Logic->GetFeePoolDomainName(i));
	};
}

void __fastcall TForm3::FillNetworkData()
{
this->Memo2->Lines->Clear();
int i = this->ComboBox4->ItemIndex;
int rig_number = this->ComboBox1->ItemIndex + 1;
UnicodeString name,mask,gateway;
name = NetworkConfigs->operator [](i).FriendlyName;
mask = NetworkConfigs->operator [](i).Mask;
gateway = NetworkConfigs->operator [](i).Gateway;
this->Memo2->Lines->Add("netsh interface ipv4 add dnsservers \""+name+"\" "+NetworkConfigs->operator [](0).DNS[0]);
if (NetworkConfigs->operator [](0).DNS.size()==2) {
	this->Memo2->Lines->Add("netsh interface ipv4 add dnsservers \""+name+"\" "+NetworkConfigs->operator [](0).DNS[1]+" index=2");
	}
this->Memo2->Lines->Add("netsh interface ipv4 add address name=\""+name+"\" address="+NetworkConfigs->operator [](i).ipAddress+" mask="+mask+" gateway="+gateway);
for (i = 0; i < Logic->GetPoolsCount(); i++) {
	this->Memo2->Lines->Add("netsh interface ipv4 add address name=\""+name+"\" address=8.8." + IntToStr(rig_number) + ".10"+IntToStr(i)+" mask=255.255.255.255 gateway="+gateway);
	}
this->Memo2->Lines->Add("pause");
}
//---------------------------------------------------------------------------
TNetworkConfig::TNetworkConfig()
{
this->AdapterName = "empty";
this->AdapterDescription = "empty";
this->FriendlyName = "empty";
this->ipAddress = "empty";
this->Mask = "empty";
this->Gateway = "empty";
this->DNS.resize(0);
}

TNetworkConfig::~TNetworkConfig()
{

}
void __fastcall TForm3::ComboBox4Change(TObject *Sender)
{
this->FillNetworkData();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::ComboBox1Change(TObject *Sender)
{
this->FillHostsData();
this->FillNetworkData();
}
//---------------------------------------------------------------------------
