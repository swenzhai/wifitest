
// wifiDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "wifi.h"
#include "wifiDlg.h"
#include "afxdialogex.h"


#include <windows.h>
#include <wlanapi.h>
#include <objbase.h>
#include <wtypes.h>

#include <stdio.h>
#include <stdlib.h>

// Need to link with Wlanapi.lib and Ole32.lib
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CwifiDlg �Ի���



CwifiDlg::CwifiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WIFI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CwifiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CwifiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CwifiDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CwifiDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CwifiDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CwifiDlg ��Ϣ�������

BOOL CwifiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CwifiDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CwifiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CwifiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LPCWSTR xml_base = L"\
<?xml version=\"1.0\"?>\r\n\
<WLANProfile xmlns = \"http://www.microsoft.com/networking/WLAN/profile/v1\">\n\
	<name>wed</name>\n\
	<SSIDConfig>\n\
		<SSID>\n\
			<name>wed</name>\n\
		</SSID>\n\
	</SSIDConfig>\n\
	<connectionType>ESS</connectionType>\n\
	<connectionMode>auto</connectionMode>\n\
	<MSM>\n\
		<security>\n\
			<authEncryption>\n\
				<authentication>WPA2PSK</authentication>\n\
				<encryption>AES</encryption>\n\
				<useOneX>false</useOneX>\n\
			</authEncryption>\n\
			<sharedKey>\n\
				<keyType>passPhrase</keyType>\n\
				<protected>false</protected>\n\
				<keyMaterial>519519wed</keyMaterial>\n\
			</sharedKey>\n\
			</security>\n\
	</MSM>\n\
	<MacRandomization xmlns = \"http://www.microsoft.com/networking/WLAN/profile/v3\">\n\
		<enableRandomization>false</enableRandomization>\n\
	</MacRandomization>\n\
</WLANProfile>";

HANDLE hClient = NULL;
DWORD dwMaxClient = 2;      //    
DWORD dwCurVersion = 0;
/* variables used for WlanEnumInterfaces  */
PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
PWLAN_INTERFACE_INFO pIfInfo = NULL;
PWLAN_AVAILABLE_NETWORK_LIST pBssList = NULL;
PWLAN_AVAILABLE_NETWORK pBssEntry = NULL;
//get profile
LPCWSTR pProfileName = L"wed";
LPWSTR pProfileXml = NULL;
DWORD dwFlags = 0;
DWORD dwGrantedAccess = 0;
void CwifiDlg::OnBnClickedButton1()
{
	DWORD dwResult = 0;
	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
	pIfInfo = (WLAN_INTERFACE_INFO *)& pIfList->InterfaceInfo[0];
	dwResult = WlanGetAvailableNetworkList(hClient,
		&pIfInfo->InterfaceGuid,
		0,
		NULL,
		&pBssList);

	//dwResult = WlanGetProfile(hClient,
	//	&pIfInfo->InterfaceGuid,
	//	pProfileName,
	//	NULL,
	//	&pProfileXml,
	//	&dwFlags,
	//	&dwGrantedAccess);

	//release Memory
	//if (pProfileXml != NULL) {
	//	WlanFreeMemory(pProfileXml);
	//	pProfileXml = NULL;
	//}
	//if (pBssList != NULL) {
	//	WlanFreeMemory(pBssList);
	//	pBssList = NULL;
	//}
	//if (pIfList != NULL) {
	//	WlanFreeMemory(pIfList);
	//	pIfList = NULL;
	//}
}


void CwifiDlg::OnBnClickedButton2()
{
	char xml_profile[2048];
	//MultiByteToWideChar();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD dwResult = 0;
	PWLAN_CONNECTION_PARAMETERS pConnectionParameters =new WLAN_CONNECTION_PARAMETERS;
	PDOT11_SSID pDot11Ssid;
	PDOT11_BSSID_LIST pDesiredBssidList;
	DOT11_BSS_TYPE dot11BssType;
	DWORD dwFlags;
	WLAN_REASON_CODE rCode;
	//sprintf(xml_profile, xml_base, 
	//	"wed");
	pConnectionParameters->wlanConnectionMode = wlan_connection_mode_profile;
	pConnectionParameters->strProfile = L"wed";
	pConnectionParameters->pDot11Ssid = NULL;
	pConnectionParameters->pDesiredBssidList = NULL;
	pConnectionParameters->dot11BssType = dot11_BSS_type_infrastructure;
	pConnectionParameters->dwFlags = WLAN_CONNECTION_HIDDEN_NETWORK;

	dwResult = WlanSetProfile(hClient,
		&pIfInfo->InterfaceGuid,
		2,
		xml_base,
		NULL,
		TRUE,
		NULL,
		&rCode
	);
		//ERROR_ACCESS_DENIED
		//ERROR_ALREADY_EXISTS
		//ERROR_BAD_PROFILE
		//ERROR_INVALID_PARAMETER
		//ERROR_NO_MATCH
		//RPC_STATUS
	dwResult = WlanConnect(hClient,
					&pIfInfo->InterfaceGuid,
					pConnectionParameters,
					NULL);

}


void CwifiDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
