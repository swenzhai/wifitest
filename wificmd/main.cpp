
#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <wlanapi.h>
#include <objbase.h>
#include <wtypes.h>

#include <stdio.h>
#include <stdlib.h>

// Need to link with Wlanapi.lib and Ole32.lib
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")

int before_wmain()
{
	// Declare and initialize variables.

	HANDLE hClient = NULL;
	DWORD dwMaxClient = 2;      //    
	DWORD dwCurVersion = 0;
	DWORD dwResult = 0;
	DWORD dwRetVal = 0;
	int iRet = 0;

	WCHAR GuidString[39] = { 0 };

	unsigned int i, k;

	// variables used for WlanEnumInterfaces

	PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
	PWLAN_INTERFACE_INFO pIfInfo = NULL;

	// variables used for WlanQueryInterfaces for opcode = wlan_intf_opcode_current_connection
	PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;
	DWORD connectInfoSize = sizeof(WLAN_CONNECTION_ATTRIBUTES);
	WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;

	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
		return 1;
		// You can use FormatMessage here to find out why the function failed
	}

	dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
		return 1;
		// You can use FormatMessage here to find out why the function failed
	}
	else {
		wprintf(L"Num Entries: %lu\n", pIfList->dwNumberOfItems);
		wprintf(L"Current Index: %lu\n", pIfList->dwIndex);
		for (i = 0; i < (int)pIfList->dwNumberOfItems; i++) {
			pIfInfo = (WLAN_INTERFACE_INFO *)& pIfList->InterfaceInfo[i];
			wprintf(L"  Interface Index[%u]:\t %lu\n", i, i);
			iRet =
				StringFromGUID2(pIfInfo->InterfaceGuid, (LPOLESTR)& GuidString,
					sizeof(GuidString) / sizeof(*GuidString));
			// For c rather than C++ source code, the above line needs to be
			// iRet = StringFromGUID2(&pIfInfo->InterfaceGuid, (LPOLESTR) &GuidString, 
			//     sizeof(GuidString)/sizeof(*GuidString)); 
			if (iRet == 0)
				wprintf(L"StringFromGUID2 failed\n");
			else {
				wprintf(L"  InterfaceGUID[%d]:\t %ws\n", i, GuidString);
			}
			wprintf(L"  Interface Description[%d]: %ws", i, pIfInfo->strInterfaceDescription);
			wprintf(L"\n");
			wprintf(L"  Interface State[%d]:\t ", i);
			switch (pIfInfo->isState) {
			case wlan_interface_state_not_ready:
				wprintf(L"Not ready\n");
				break;
			case wlan_interface_state_connected:
				wprintf(L"Connected\n");
				break;
			case wlan_interface_state_ad_hoc_network_formed:
				wprintf(L"First node in a ad hoc network\n");
				break;
			case wlan_interface_state_disconnecting:
				wprintf(L"Disconnecting\n");
				break;
			case wlan_interface_state_disconnected:
				wprintf(L"Not connected\n");
				break;
			case wlan_interface_state_associating:
				wprintf(L"Attempting to associate with a network\n");
				break;
			case wlan_interface_state_discovering:
				wprintf(L"Auto configuration is discovering settings for the network\n");
				break;
			case wlan_interface_state_authenticating:
				wprintf(L"In process of authenticating\n");
				break;
			default:
				wprintf(L"Unknown state %ld\n", pIfInfo->isState);
				break;
			}
			wprintf(L"\n");

			// If interface state is connected, call WlanQueryInterface
			// to get current connection attributes
			if (pIfInfo->isState == wlan_interface_state_connected) {
				dwResult = WlanQueryInterface(hClient,
					&pIfInfo->InterfaceGuid,
					wlan_intf_opcode_current_connection,
					NULL,
					&connectInfoSize,
					(PVOID *)&pConnectInfo,
					&opCode);

				if (dwResult != ERROR_SUCCESS) {
					wprintf(L"WlanQueryInterface failed with error: %u\n", dwResult);
					dwRetVal = 1;
					// You can use FormatMessage to find out why the function failed
				}
				else {
					wprintf(L"  WLAN_CONNECTION_ATTRIBUTES for this interface\n");

					wprintf(L"  Interface State:\t ");
					switch (pConnectInfo->isState) {
					case wlan_interface_state_not_ready:
						wprintf(L"Not ready\n");
						break;
					case wlan_interface_state_connected:
						wprintf(L"Connected\n");
						break;
					case wlan_interface_state_ad_hoc_network_formed:
						wprintf(L"First node in a ad hoc network\n");
						break;
					case wlan_interface_state_disconnecting:
						wprintf(L"Disconnecting\n");
						break;
					case wlan_interface_state_disconnected:
						wprintf(L"Not connected\n");
						break;
					case wlan_interface_state_associating:
						wprintf(L"Attempting to associate with a network\n");
						break;
					case wlan_interface_state_discovering:
						wprintf
						(L"Auto configuration is discovering settings for the network\n");
						break;
					case wlan_interface_state_authenticating:
						wprintf(L"In process of authenticating\n");
						break;
					default:
						wprintf(L"Unknown state %ld\n", pIfInfo->isState);
						break;
					}

					wprintf(L"  Connection Mode:\t ");
					switch (pConnectInfo->wlanConnectionMode) {
					case wlan_connection_mode_profile:
						wprintf(L"A profile is used to make the connection\n");
						break;
					case wlan_connection_mode_temporary_profile:
						wprintf(L"A temporary profile is used to make the connection\n");
						break;
					case wlan_connection_mode_discovery_secure:
						wprintf(L"Secure discovery is used to make the connection\n");
						break;
					case wlan_connection_mode_discovery_unsecure:
						wprintf(L"Unsecure discovery is used to make the connection\n");
						break;
					case wlan_connection_mode_auto:
						wprintf
						(L"connection initiated by wireless service automatically using a persistent profile\n");
						break;
					case wlan_connection_mode_invalid:
						wprintf(L"Invalid connection mode\n");
						break;
					default:
						wprintf(L"Unknown connection mode %ld\n",
							pConnectInfo->wlanConnectionMode);
						break;
					}

					wprintf(L"  Profile name used:\t %ws\n", pConnectInfo->strProfileName);

					wprintf(L"  Association Attributes for this connection\n");
					wprintf(L"    SSID:\t\t ");
					if (pConnectInfo->wlanAssociationAttributes.dot11Ssid.uSSIDLength == 0)
						wprintf(L"\n");
					else {
						for (k = 0;
							k < pConnectInfo->wlanAssociationAttributes.dot11Ssid.uSSIDLength;
							k++) {
							wprintf(L"%c",
								(int)pConnectInfo->wlanAssociationAttributes.dot11Ssid.
								ucSSID[k]);
						}
						wprintf(L"\n");
					}

					wprintf(L"    BSS Network type:\t ");
					switch (pConnectInfo->wlanAssociationAttributes.dot11BssType) {
					case dot11_BSS_type_infrastructure:
						wprintf(L"Infrastructure\n");
						break;
					case dot11_BSS_type_independent:
						wprintf(L"Infrastructure\n");
						break;
					default:
						wprintf(L"Other = %lu\n",
							pConnectInfo->wlanAssociationAttributes.dot11BssType);
						break;
					}

					wprintf(L"    MAC address:\t ");
					for (k = 0; k < sizeof(pConnectInfo->wlanAssociationAttributes.dot11Bssid);
						k++) {
						if (k == 5)
							wprintf(L"%.2X\n",
								pConnectInfo->wlanAssociationAttributes.dot11Bssid[k]);
						else
							wprintf(L"%.2X-",
								pConnectInfo->wlanAssociationAttributes.dot11Bssid[k]);
					}

					wprintf(L"    PHY network type:\t ");
					switch (pConnectInfo->wlanAssociationAttributes.dot11PhyType) {
					case dot11_phy_type_fhss:
						wprintf(L"Frequency-hopping spread-spectrum (FHSS)\n");
						break;
					case dot11_phy_type_dsss:
						wprintf(L"Direct sequence spread spectrum (DSSS)\n");
						break;
					case dot11_phy_type_irbaseband:
						wprintf(L"Infrared (IR) baseband\n");
						break;
					case dot11_phy_type_ofdm:
						wprintf(L"Orthogonal frequency division multiplexing (OFDM)\n");
						break;
					case dot11_phy_type_hrdsss:
						wprintf(L"High-rate DSSS (HRDSSS) = \n");
						break;
					case dot11_phy_type_erp:
						wprintf(L"Extended rate PHY type\n");
						break;
					case dot11_phy_type_ht:
						wprintf(L"802.11n PHY type\n");
						break;
					default:
						wprintf(L"Unknown = %lu\n",
							pConnectInfo->wlanAssociationAttributes.dot11PhyType);
						break;
					}

					wprintf(L"    PHY index:\t\t %u\n",
						pConnectInfo->wlanAssociationAttributes.uDot11PhyIndex);

					wprintf(L"    Signal Quality:\t %d\n",
						pConnectInfo->wlanAssociationAttributes.wlanSignalQuality);

					wprintf(L"    Receiving Rate:\t %ld\n",
						pConnectInfo->wlanAssociationAttributes.ulRxRate);

					wprintf(L"    Transmission Rate:\t %ld\n",
						pConnectInfo->wlanAssociationAttributes.ulTxRate);
					wprintf(L"\n");

					wprintf(L"  Security Attributes for this connection\n");

					wprintf(L"    Security enabled:\t ");
					if (pConnectInfo->wlanSecurityAttributes.bSecurityEnabled == 0)
						wprintf(L"No\n");
					else
						wprintf(L"Yes\n");

					wprintf(L"    802.1X enabled:\t ");
					if (pConnectInfo->wlanSecurityAttributes.bOneXEnabled == 0)
						wprintf(L"No\n");
					else
						wprintf(L"Yes\n");

					wprintf(L"    Authentication Algorithm: ");
					switch (pConnectInfo->wlanSecurityAttributes.dot11AuthAlgorithm) {
					case DOT11_AUTH_ALGO_80211_OPEN:
						wprintf(L"802.11 Open\n");
						break;
					case DOT11_AUTH_ALGO_80211_SHARED_KEY:
						wprintf(L"802.11 Shared\n");
						break;
					case DOT11_AUTH_ALGO_WPA:
						wprintf(L"WPA\n");
						break;
					case DOT11_AUTH_ALGO_WPA_PSK:
						wprintf(L"WPA-PSK\n");
						break;
					case DOT11_AUTH_ALGO_WPA_NONE:
						wprintf(L"WPA-None\n");
						break;
					case DOT11_AUTH_ALGO_RSNA:
						wprintf(L"RSNA\n");
						break;
					case DOT11_AUTH_ALGO_RSNA_PSK:
						wprintf(L"RSNA with PSK\n");
						break;
					default:
						wprintf(L"Other (%lu)\n", pConnectInfo->wlanSecurityAttributes.dot11AuthAlgorithm);
						break;
					}

					wprintf(L"    Cipher Algorithm:\t ");
					switch (pConnectInfo->wlanSecurityAttributes.dot11CipherAlgorithm) {
					case DOT11_CIPHER_ALGO_NONE:
						wprintf(L"None\n");
						break;
					case DOT11_CIPHER_ALGO_WEP40:
						wprintf(L"WEP-40\n");
						break;
					case DOT11_CIPHER_ALGO_TKIP:
						wprintf(L"TKIP\n");
						break;
					case DOT11_CIPHER_ALGO_CCMP:
						wprintf(L"CCMP\n");
						break;
					case DOT11_CIPHER_ALGO_WEP104:
						wprintf(L"WEP-104\n");
						break;
					case DOT11_CIPHER_ALGO_WEP:
						wprintf(L"WEP\n");
						break;
					default:
						wprintf(L"Other (0x%x)\n", pConnectInfo->wlanSecurityAttributes.dot11CipherAlgorithm);
						break;
					}
					wprintf(L"\n");
				}
			}
		}

	}
	if (pConnectInfo != NULL) {
		WlanFreeMemory(pConnectInfo);
		pConnectInfo = NULL;
	}

	if (pIfList != NULL) {
		WlanFreeMemory(pIfList);
		pIfList = NULL;
	}

	return dwRetVal;
}
int get_ssid() {
	// Declare and initialize variables.

	HANDLE hClient = NULL;
	DWORD dwMaxClient = 2;      //    
	DWORD dwCurVersion = 0;
	DWORD dwResult = 0;
	DWORD dwRetVal = 0;
	int iRet = 0;

	WCHAR GuidString[39] = { 0 };

	unsigned int i, j, k;

	/* variables used for WlanEnumInterfaces  */

	PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
	PWLAN_INTERFACE_INFO pIfInfo = NULL;

	PWLAN_AVAILABLE_NETWORK_LIST pBssList = NULL;
	PWLAN_AVAILABLE_NETWORK pBssEntry = NULL;

	int iRSSI = 0;

	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
		return 1;
		// You can use FormatMessage here to find out why the function failed
	}

	dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
		return 1;
		// You can use FormatMessage here to find out why the function failed
	}
	else {
		wprintf(L"Num Entries: %lu\n", pIfList->dwNumberOfItems);
		wprintf(L"Current Index: %lu\n", pIfList->dwIndex);
		for (i = 0; i < (int)pIfList->dwNumberOfItems; i++) {
			pIfInfo = (WLAN_INTERFACE_INFO *)&pIfList->InterfaceInfo[i];
			wprintf(L"  Interface Index[%u]:\t %lu\n", i, i);
			iRet = StringFromGUID2(pIfInfo->InterfaceGuid, (LPOLESTR)&GuidString,
				sizeof(GuidString) / sizeof(*GuidString));
			// For c rather than C++ source code, the above line needs to be
			// iRet = StringFromGUID2(&pIfInfo->InterfaceGuid, (LPOLESTR) &GuidString, 
			//     sizeof(GuidString)/sizeof(*GuidString)); 
			if (iRet == 0)
				wprintf(L"StringFromGUID2 failed\n");
			else {
				wprintf(L"  InterfaceGUID[%d]: %ws\n", i, GuidString);
			}
			wprintf(L"  Interface Description[%d]: %ws", i,
				pIfInfo->strInterfaceDescription);
			wprintf(L"\n");
			wprintf(L"  Interface State[%d]:\t ", i);
			switch (pIfInfo->isState) {
			case wlan_interface_state_not_ready:
				wprintf(L"Not ready\n");
				break;
			case wlan_interface_state_connected:
				wprintf(L"Connected\n");
				break;
			case wlan_interface_state_ad_hoc_network_formed:
				wprintf(L"First node in a ad hoc network\n");
				break;
			case wlan_interface_state_disconnecting:
				wprintf(L"Disconnecting\n");
				break;
			case wlan_interface_state_disconnected:
				wprintf(L"Not connected\n");
				break;
			case wlan_interface_state_associating:
				wprintf(L"Attempting to associate with a network\n");
				break;
			case wlan_interface_state_discovering:
				wprintf(L"Auto configuration is discovering settings for the network\n");
				break;
			case wlan_interface_state_authenticating:
				wprintf(L"In process of authenticating\n");
				break;
			default:
				wprintf(L"Unknown state %ld\n", pIfInfo->isState);
				break;
			}
			wprintf(L"\n");

			dwResult = WlanGetAvailableNetworkList(hClient,
				&pIfInfo->InterfaceGuid,
				0,
				NULL,
				&pBssList);

			if (dwResult != ERROR_SUCCESS) {
				wprintf(L"WlanGetAvailableNetworkList failed with error: %u\n",
					dwResult);
				dwRetVal = 1;
				// You can use FormatMessage to find out why the function failed
			}
			else {
				wprintf(L"WLAN_AVAILABLE_NETWORK_LIST for this interface\n");

				wprintf(L"  Num Entries: %lu\n\n", pBssList->dwNumberOfItems);

				for (j = 0; j < pBssList->dwNumberOfItems; j++) {
					pBssEntry =
						(WLAN_AVAILABLE_NETWORK *)& pBssList->Network[j];

					wprintf(L"  Profile Name[%u]:  %ws\n", j, pBssEntry->strProfileName);

					wprintf(L"  SSID[%u]:\t\t ", j);
					if (pBssEntry->dot11Ssid.uSSIDLength == 0)
						wprintf(L"\n");
					else {
						for (k = 0; k < pBssEntry->dot11Ssid.uSSIDLength; k++) {
							wprintf(L"%c", (int)pBssEntry->dot11Ssid.ucSSID[k]);
						}
						wprintf(L"\n");
					}

					wprintf(L"  BSS Network type[%u]:\t ", j);
					switch (pBssEntry->dot11BssType) {
					case dot11_BSS_type_infrastructure:
						wprintf(L"Infrastructure (%u)\n", pBssEntry->dot11BssType);
						break;
					case dot11_BSS_type_independent:
						wprintf(L"Infrastructure (%u)\n", pBssEntry->dot11BssType);
						break;
					default:
						wprintf(L"Other (%lu)\n", pBssEntry->dot11BssType);
						break;
					}

					wprintf(L"  Number of BSSIDs[%u]:\t %u\n", j, pBssEntry->uNumberOfBssids);

					wprintf(L"  Connectable[%u]:\t ", j);
					if (pBssEntry->bNetworkConnectable)
						wprintf(L"Yes\n");
					else {
						wprintf(L"No\n");
						wprintf(L"  Not connectable WLAN_REASON_CODE value[%u]:\t %u\n", j,
							pBssEntry->wlanNotConnectableReason);
					}

					wprintf(L"  Number of PHY types supported[%u]:\t %u\n", j, pBssEntry->uNumberOfPhyTypes);

					if (pBssEntry->wlanSignalQuality == 0)
						iRSSI = -100;
					else if (pBssEntry->wlanSignalQuality == 100)
						iRSSI = -50;
					else
						iRSSI = -100 + (pBssEntry->wlanSignalQuality / 2);

					wprintf(L"  Signal Quality[%u]:\t %u (RSSI: %i dBm)\n", j,
						pBssEntry->wlanSignalQuality, iRSSI);

					wprintf(L"  Security Enabled[%u]:\t ", j);
					if (pBssEntry->bSecurityEnabled)
						wprintf(L"Yes\n");
					else
						wprintf(L"No\n");

					wprintf(L"  Default AuthAlgorithm[%u]: ", j);
					switch (pBssEntry->dot11DefaultAuthAlgorithm) {
					case DOT11_AUTH_ALGO_80211_OPEN:
						wprintf(L"802.11 Open (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
						break;
					case DOT11_AUTH_ALGO_80211_SHARED_KEY:
						wprintf(L"802.11 Shared (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
						break;
					case DOT11_AUTH_ALGO_WPA:
						wprintf(L"WPA (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
						break;
					case DOT11_AUTH_ALGO_WPA_PSK:
						wprintf(L"WPA-PSK (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
						break;
					case DOT11_AUTH_ALGO_WPA_NONE:
						wprintf(L"WPA-None (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
						break;
					case DOT11_AUTH_ALGO_RSNA:
						wprintf(L"RSNA (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
						break;
					case DOT11_AUTH_ALGO_RSNA_PSK:
						wprintf(L"RSNA with PSK(%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
						break;
					default:
						wprintf(L"Other (%lu)\n", pBssEntry->dot11DefaultAuthAlgorithm);
						break;
					}

					wprintf(L"  Default CipherAlgorithm[%u]: ", j);
					switch (pBssEntry->dot11DefaultCipherAlgorithm) {
					case DOT11_CIPHER_ALGO_NONE:
						wprintf(L"None (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
						break;
					case DOT11_CIPHER_ALGO_WEP40:
						wprintf(L"WEP-40 (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
						break;
					case DOT11_CIPHER_ALGO_TKIP:
						wprintf(L"TKIP (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
						break;
					case DOT11_CIPHER_ALGO_CCMP:
						wprintf(L"CCMP (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
						break;
					case DOT11_CIPHER_ALGO_WEP104:
						wprintf(L"WEP-104 (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
						break;
					case DOT11_CIPHER_ALGO_WEP:
						wprintf(L"WEP (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
						break;
					default:
						wprintf(L"Other (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
						break;
					}

					wprintf(L"  Flags[%u]:\t 0x%x", j, pBssEntry->dwFlags);
					if (pBssEntry->dwFlags) {
						if (pBssEntry->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED)
							wprintf(L" - Currently connected");
						if (pBssEntry->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED)
							wprintf(L" - Has profile");
					}
					wprintf(L"\n");

					wprintf(L"\n");
				}
			}
		}

	}
	if (pBssList != NULL) {
		WlanFreeMemory(pBssList);
		pBssList = NULL;
	}

	if (pIfList != NULL) {
		WlanFreeMemory(pIfList);
		pIfList = NULL;
	}

	return dwRetVal;
}
int get_profile(int argc, WCHAR **argv)
{

	// Declare and initialize variables.

	HANDLE hClient = NULL;
	DWORD dwMaxClient = 2;      //    
	DWORD dwCurVersion = 0;
	DWORD dwResult = 0;
	DWORD dwRetVal = 0;
	int iRet = 0;

	WCHAR GuidString[39] = { 0 };

	unsigned int i;

	/* variables used for WlanEnumInterfaces  */

	PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
	PWLAN_INTERFACE_INFO pIfInfo = NULL;

	LPCWSTR pProfileName = NULL;
	LPWSTR pProfileXml = NULL;
	DWORD dwFlags = 0;
	DWORD dwGrantedAccess = 0;

	// Validate the parameters
	if (argc < 2) {
		wprintf(L"usage: %s <profile>\n", argv[0]);
		wprintf(L"   Gets a wireless profile\n");
		wprintf(L"   Example\n");
		wprintf(L"       %s \"Default Wireless\"\n", argv[0]);
		exit(1);
	}

	pProfileName = argv[1];

	wprintf(L"Information for profile: %ws\n\n", pProfileName);

	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
		return 1;
		// You can use FormatMessage here to find out why the function failed
	}

	dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
		return 1;
		// You can use FormatMessage here to find out why the function failed
	}
	else {
		wprintf(L"WLAN_INTERFACE_INFO_LIST for this system\n");

		wprintf(L"Num Entries: %lu\n", pIfList->dwNumberOfItems);
		wprintf(L"Current Index: %lu\n\n", pIfList->dwIndex);
		for (i = 0; i < (int)pIfList->dwNumberOfItems; i++) {
			pIfInfo = (WLAN_INTERFACE_INFO *)&pIfList->InterfaceInfo[i];
			wprintf(L"  Interface Index[%u]:\t %lu\n", i, i);
			iRet = StringFromGUID2(pIfInfo->InterfaceGuid, (LPOLESTR)&GuidString,
				sizeof(GuidString) / sizeof(*GuidString));
			// For c rather than C++ source code, the above line needs to be
			// iRet = StringFromGUID2(&pIfInfo->InterfaceGuid, (LPOLESTR) &GuidString, 
			//     sizeof(GuidString)/sizeof(*GuidString)); 
			if (iRet == 0)
				wprintf(L"StringFromGUID2 failed\n");
			else {
				wprintf(L"  InterfaceGUID[%d]: %ws\n", i, GuidString);
			}
			wprintf(L"  Interface Description[%d]: %ws", i,
				pIfInfo->strInterfaceDescription);
			wprintf(L"\n");
			wprintf(L"  Interface State[%d]:\t ", i);
			switch (pIfInfo->isState) {
			case wlan_interface_state_not_ready:
				wprintf(L"Not ready\n");
				break;
			case wlan_interface_state_connected:
				wprintf(L"Connected\n");
				break;
			case wlan_interface_state_ad_hoc_network_formed:
				wprintf(L"First node in a ad hoc network\n");
				break;
			case wlan_interface_state_disconnecting:
				wprintf(L"Disconnecting\n");
				break;
			case wlan_interface_state_disconnected:
				wprintf(L"Not connected\n");
				break;
			case wlan_interface_state_associating:
				wprintf(L"Attempting to associate with a network\n");
				break;
			case wlan_interface_state_discovering:
				wprintf(L"Auto configuration is discovering settings for the network\n");
				break;
			case wlan_interface_state_authenticating:
				wprintf(L"In process of authenticating\n");
				break;
			default:
				wprintf(L"Unknown state %ld\n", pIfInfo->isState);
				break;
			}
			wprintf(L"\n\n");

			dwResult = WlanGetProfile(hClient,
				&pIfInfo->InterfaceGuid,
				pProfileName,
				NULL,
				&pProfileXml,
				&dwFlags,
				&dwGrantedAccess);

			if (dwResult != ERROR_SUCCESS) {
				wprintf(L"WlanGetProfile failed with error: %u\n",
					dwResult);
				// You can use FormatMessage to find out why the function failed
			}
			else {
				wprintf(L"  Profile Name:  %ws\n", pProfileName);

				wprintf(L"  Profile XML string:\n");
				wprintf(L"%ws\n\n", pProfileXml);

				wprintf(L"  dwFlags:\t    0x%x", dwFlags);
				//                    if (dwFlags & WLAN_PROFILE_GET_PLAINTEXT_KEY)
				//                        wprintf(L"   Get Plain Text Key");
				if (dwFlags & WLAN_PROFILE_GROUP_POLICY)
					wprintf(L"  Group Policy");
				if (dwFlags & WLAN_PROFILE_USER)
					wprintf(L"  Per User Profile");
				wprintf(L"\n");

				wprintf(L"  dwGrantedAccess:  0x%x", dwGrantedAccess);
				if (dwGrantedAccess & WLAN_READ_ACCESS)
					wprintf(L"  Read access");
				if (dwGrantedAccess & WLAN_EXECUTE_ACCESS)
					wprintf(L"  Execute access");
				if (dwGrantedAccess & WLAN_WRITE_ACCESS)
					wprintf(L"  Write access");
				wprintf(L"\n");

				wprintf(L"\n");
			}
		}

	}
	if (pProfileXml != NULL) {
		WlanFreeMemory(pProfileXml);
		pProfileXml = NULL;
	}

	if (pIfList != NULL) {
		WlanFreeMemory(pIfList);
		pIfList = NULL;
	}

	return dwRetVal;
}
int del_profile(int argc, WCHAR ** argv)
{

	// Declare and initialize variables.

	HANDLE hClient = NULL;
	DWORD dwMaxClient = 2;      //    
	DWORD dwCurVersion = 0;
	DWORD dwResult = 0;
	DWORD dwRetVal = 0;
	int iRet = 0;

	WCHAR GuidString[39] = { 0 };

	unsigned int i;

	/* variables used for WlanEnumInterfaces  */

	PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
	PWLAN_INTERFACE_INFO pIfInfo = NULL;

	LPCWSTR pProfileName = NULL;

	// Validate the parameters
	if (argc < 2) {
		wprintf(L"usage: %s <profile>\n", argv[0]);
		wprintf(L"   Deletes a wireless profile\n");
		wprintf(L"   Example\n");
		wprintf(L"       %s \"Default Wireless\"\n", argv[0]);
		exit(1);
	}

	pProfileName = argv[1];

	wprintf(L"Information for profile: %ws\n\n", pProfileName);

	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
		return 1;
		// You can use FormatMessage here to find out why the function failed
	}

	dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
		return 1;
		// You can use FormatMessage here to find out why the function failed
	}
	else {
		wprintf(L"WLAN_INTERFACE_INFO_LIST for this system\n");

		wprintf(L"Num Entries: %lu\n", pIfList->dwNumberOfItems);
		wprintf(L"Current Index: %lu\n", pIfList->dwIndex);
		for (i = 0; i < pIfList->dwNumberOfItems; i++) {
			pIfInfo = (WLAN_INTERFACE_INFO *)& pIfList->InterfaceInfo[i];
			wprintf(L"  Interface Index[%u]:\t %lu\n", i, i);
			iRet =
				StringFromGUID2(pIfInfo->InterfaceGuid, (LPOLESTR)& GuidString,
					sizeof(GuidString) / sizeof(*GuidString));
			// For c rather than C++ source code, the above line needs to be
			// iRet = StringFromGUID2(&pIfInfo->InterfaceGuid, (LPOLESTR) &GuidString, 
			//     sizeof(GuidString)/sizeof(*GuidString)); 
			if (iRet == 0)
				wprintf(L"StringFromGUID2 failed\n");
			else {
				wprintf(L"  InterfaceGUID[%d]: %ws\n", i, GuidString);
			}
			wprintf(L"  Interface Description[%d]: %ws", i,
				pIfInfo->strInterfaceDescription);
			wprintf(L"\n");
			wprintf(L"  Interface State[%d]:\t ", i);
			switch (pIfInfo->isState) {
			case wlan_interface_state_not_ready:
				wprintf(L"Not ready\n");
				break;
			case wlan_interface_state_connected:
				wprintf(L"Connected\n");
				break;
			case wlan_interface_state_ad_hoc_network_formed:
				wprintf(L"First node in a ad hoc network\n");
				break;
			case wlan_interface_state_disconnecting:
				wprintf(L"Disconnecting\n");
				break;
			case wlan_interface_state_disconnected:
				wprintf(L"Not connected\n");
				break;
			case wlan_interface_state_associating:
				wprintf(L"Attempting to associate with a network\n");
				break;
			case wlan_interface_state_discovering:
				wprintf
				(L"Auto configuration is discovering settings for the network\n");
				break;
			case wlan_interface_state_authenticating:
				wprintf(L"In process of authenticating\n");
				break;
			default:
				wprintf(L"Unknown state %ld\n", pIfInfo->isState);
				break;
			}
			wprintf(L"\n");

			dwResult = WlanDeleteProfile(hClient,
				&pIfInfo->InterfaceGuid,
				pProfileName, NULL);

			if (dwResult != ERROR_SUCCESS) {
				wprintf
				(L"WlanDeleteProfile failed on this interface with error: %u\n",
					dwResult);
				if (dwResult == ERROR_NOT_FOUND)
					wprintf
					(L"  Error was the following profile was not found: %ws\n",
						pProfileName);
				// You can use FormatMessage to find out why the function failed
			}
			else {
				wprintf(L"Successfully deleted Profile Name: %ws\n",
					pProfileName);
			}
			wprintf(L"\n");
		}

	}
	if (pIfList != NULL) {
		WlanFreeMemory(pIfList);
		pIfList = NULL;
	}

	return dwRetVal;
}
//used in win7
int test() {
	//-----------------------------------------
	// Declare and initialize variables
	HINSTANCE nResult;

	PCWSTR lpOperation = L"open";
	PCWSTR lpFile =
		L"shell:::{26EE0668-A00A-44D7-9371-BEB064C98683}\\3\\::{1fa9085f-25a2-489b-85d4-86326eedcd87}";

	nResult = ShellExecute(
		NULL,   // Hwnd
		lpOperation, // do not request elevation unless needed
		lpFile,
		NULL, // no parameters 
		NULL, // use current working directory 
		SW_SHOW);//SW_SHOWNORMAL);

	if ((int)nResult == SE_ERR_ACCESSDENIED)
	{
		wprintf(L"ShellExecute returned access denied\n");
		wprintf(L"  Executing the ShellExecute command elevated\n");

		nResult = ShellExecute(
			NULL,
			L"runas", // Trick for requesting elevation
			lpFile,
			NULL, // no parameters 
			NULL, // use current working directory 
			SW_SHOW);// SW_HIDE);
	}

	if ((int)nResult < 32) {
		wprintf(L" ShellExecute failed with error %d\n", (int)nResult);
		return 1;
	}
	else {
		wprintf(L" ShellExecute succeeded and returned value %d\n", (int)nResult);
		return 0;
	}
}

int wmain() {
	//before_wmain();
	get_ssid();
	//get_profile();
	return 1;
}