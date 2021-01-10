// SystemInfo.cpp: implementation of the SystemInfo class.
//////////////////////////////////////////////////////////////////////

#include "SystemInfo.h"

#include <tchar.h>
#include <stdio.h>

#ifdef __CYGWIN__
#define _tcscpy strcpy
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define BUFSIZE 80

#ifndef VER_SUITE_WH_SERVER
#define VER_SUITE_WH_SERVER 0x8000
#endif

typedef void (WINAPI *PGetNativeSystemInfo)(LPSYSTEM_INFO);

typedef BOOL (WINAPI *PGetProductInfo)(DWORD, DWORD, DWORD, DWORD, PDWORD);

const char *WindowsVersionStr[] = {
    "Windows",
    "Windows 32s",
    "Windows 95",
    "Windows 95 SR2",
    "Windows 98",
    "Windows 98 SE",
    "Windows Me",
    "Windows NT 3.51",
    "Windows NT 4.0",
    "Windows NT 4.0 Server",
    "Windows 2000",
    "Windows XP",
    "Windows XP Professional x64",
    "Windows Home Server",
    "Windows Server 2003",
    "Windows Server 2003 R2",
    "Windows Vista",
    "Windows Server 2008",
    "Windows 7",
    "Windows Server 2008 R2",
    "Windows 8",
    "Windows Server 2012",
    "Windows 8.1",
    "Windows Server 2012 R2",
    "Windows 10",
    "Windows Server 2016"
};

const char *WindowsEditionStr[] = {
    "Unknown product",
    "Work Station",
    "Server",
    "Advanced Server",
    "Home",
    "Ultimate",
    "Home Basic",
    "Home Premium",
    "Enterprise",
    "Home Basic N",
    "Business",
    "Server Standard",
    "Server Datacenter (full installation)",
    "Small Business Server",
    "Server Enterprise (full installation)",
    "Starter",
    "Server Datacenter (core installation)",
    "Server Standard (core installation)",
    "Server Enterprise (core installation)",
    "Server Enterprise for Itanium-based Systems",
    "Business N",
    "Web Server (full installation)",
    "HPC Edition",
    "Storage Essentials",
    "Storage Server Express",
    "Storage Server Standard",
    "Storage Server Workgroup",
    "Storage Server Enterprise",
    "Server for Windows Essential Server Solutions",
    "Small Business Server Premium",
    "Home Premium N",
    "Enterprise N",
    "Ultimate N",
    "Web Server (core installation)",
    "Essential Business Server Management Server",
    "Essential Business Server Security Server",
    "Essential Business Server Messaging Server",
    "Server Foundation",
    "Home Server 2011",
    "Server without Hyper-V for Windows Essential Server Solutions",
    "Server Standard without Hyper-V",
    "Server Datacenter without Hyper-V (full installation)",
    "Server Enterprise without Hyper-V (full installation)",
    "Server Datacenter without Hyper-V (core installation)",
    "Server Standard without Hyper-V (core installation)",
    "Server Enterprise without Hyper-V (core installation)",
    "Microsoft Hyper-V Server",
    "Storage Server Express (core installation)",
    "Storage Server Standard (core installation)",
    "Storage Server Workgroup (core installation)",
    "Storage Server Enterprise (core installation)",
    "Starter N",
    "Professional",
    "Professional N",
    "Small Business Server 2011 Essentials",
    "Server For Small Business Solutions",
    "Server Solutions Premium",
    "Server Solutions Premium (core installation)",
    "Server For Small Business Solutions EM",
    "Server For Small Business Solutions EM",
    "MultiPoint Server",
    "Essential Server Solution Management",
    "Essential Server Solution core",
    "Essential Server Solution Additional",
    "Essential Server Solution Management SVC",
    "Essential Server Solution Additional SVC",
    "Small Business Server Premium (core installation)",
    "Server Hyper Core V",
    "Essential Server Solution",
    "Starter E",
    "Home Basic E",
    "Home Pemiux E",
    "Professional E",
    "Enterprise E",
    "Ultimate E",
    "Enterprise Evaluation",
    "MultiPoint Server Standard (full installation)",
    "MultiPoint Server Premium (full installation)",
    "Server Standard (evaluation installation)",
    "Server Datacenter (evaluation installation)",
    "Enterprise N Evaluation",
    "Essential Server Solution Automotive",
    "Essential Server Solution Industry A",
    "Thinpc",
    "Essential Server Solution A",
    "Essential Server Solution Industry",
    "Essential Server Solution E",
    "Essential Server Solution Industry E",
    "Essential Server Solution Industry A E",
    "Storage Server Workgroup (evaluation installation)",
    "Storage Server Standard (evaluation installation)",
    "Home ARM",
    "Home N",
    "Home China",
    "Home Single Language",
    "Home",
    "Professional with Media Center",
    "Mobile",
    "Education",
    "Education N",
    "Enterprise 2015 LTSB",
    "Enterprise 2015 LTSB N",
    "Enterprise 2015 LTSB Evaluation",
    "Enterprise 2015 LTSB N Evaluation",
    "Mobile Enterprise"
};

SystemInfo::SystemInfo()
{
    BOOL canDetect = TRUE;
    PGetNativeSystemInfo pGNSI = NULL;
    m_bOsVersionInfoEx = FALSE;
    m_nWinVersion = Windows;
    m_nWinEdition = EditionUnknown;

    memset(m_szServicePack, 0, sizeof(m_szServicePack));
    /**
     * Try calling GetVersionEx using the OSVERSIONINFOEX structure.
     */
    ZeroMemory(&m_osvi, sizeof(OSVERSIONINFOEX));
    m_osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    if (!(m_bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &m_osvi)))
    {
        /**
         * If that fails, try using the OSVERSIONINFO structure.
         */
        m_osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
        if (!GetVersionEx((OSVERSIONINFO *) &m_osvi))
            canDetect = FALSE;
    }
    pGNSI = (PGetNativeSystemInfo) GetProcAddress(
        GetModuleHandle(_T("kernel32.dll")),
        "GetNativeSystemInfo");
    if (NULL != pGNSI)
        pGNSI(&m_SysInfo);
    else
        GetSystemInfo(&m_SysInfo);
    if (canDetect)
    {
        DetectWindowsVersion();
        DetectWindowsEdition();
        DetectWindowsServicePack();
        DetectCurrentVersionKeys();
    }
}

SystemInfo::~SystemInfo()
{

}

void SystemInfo::DetectWindowsVersion()
{
   if (m_bOsVersionInfoEx)
   {
      switch (m_osvi.dwPlatformId)
      {
        case VER_PLATFORM_WIN32s:
          m_nWinVersion = Windows32s;
          break;
        case VER_PLATFORM_WIN32_WINDOWS:
          /**
           * Test for the Windows 95 product family.
           */
          switch (m_osvi.dwMajorVersion)
          {
            case 4:
              switch(m_osvi.dwMinorVersion)
              {
                case 0:
                  if (m_osvi.szCSDVersion[0] == 'B' || m_osvi.szCSDVersion[0] == 'C')
                      m_nWinVersion = Windows95OSR2;
                  else
                      m_nWinVersion = Windows95;
                  break;
                case 10:
                  if (m_osvi.szCSDVersion[0] == 'A')
                      m_nWinVersion = Windows98SE;
                  else
                      m_nWinVersion = Windows98;
                  break;
                case 90:
                  m_nWinVersion = WindowsMillennium;
                  break;
              }
              break;
          }
          break;
        case VER_PLATFORM_WIN32_NT:
          /**
           * Test for the Windows NT product family.
           */
          switch (m_osvi.dwMajorVersion)
          {
            case 3:
              m_nWinVersion = WindowsNT351;
              break;
            case 4:
              switch (m_osvi.wProductType)
              {
                case 1:
                  m_nWinVersion = WindowsNT40;
                  break;
                case 3:
                  m_nWinVersion = WindowsNT40Server;
                  break;
              }
              break;
            case 5:
              switch (m_osvi.dwMinorVersion)
              {
                case 0:
                  m_nWinVersion = Windows2000;
                  break;
                case 1:
                  m_nWinVersion = WindowsXP;
                  break;
                case 2:
                  if (m_osvi.wSuiteMask == VER_SUITE_WH_SERVER)
                  {
                      m_nWinVersion = WindowsHomeServer;
                  }
                  else if (m_osvi.wProductType == VER_NT_WORKSTATION &&
                           m_SysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
                  {
                      m_nWinVersion = WindowsXPProfessionalx64;
                  }
                  else
                  {
                      m_nWinVersion = ::GetSystemMetrics(SM_SERVERR2) == 0 ?
                          WindowsServer2003 :
                          WindowsServer2003R2;
                  }
                  break;
              }
              break;
            case 6:
              switch (m_osvi.dwMinorVersion)
              {
                case 0:
                  m_nWinVersion = m_osvi.wProductType == VER_NT_WORKSTATION ?
                      WindowsVista :
                  WindowsServer2008;
                  break;
                case 1:
                  m_nWinVersion = m_osvi.wProductType == VER_NT_WORKSTATION ?
                      Windows7 :
                  WindowsServer2008R2;
                  break;
                case 2:
                  m_nWinVersion = m_osvi.wProductType == VER_NT_WORKSTATION ?
                      Windows8 :
                  WindowsServer2012;
                  break;
                case 3:
                  m_nWinVersion = m_osvi.wProductType == VER_NT_WORKSTATION ?
                      Windows8_1 :
                  WindowsServer2012R2;
                  break;
              }
              break;
            case 10:
              switch (m_osvi.dwMinorVersion)
              {
                case 0:
                  m_nWinVersion = m_osvi.wProductType == VER_NT_WORKSTATION ?
                      Windows10 :
                  WindowsServer2016;
                  break;
              }
              break;
          }
          break;
      }
   }
   else
   {
       /**
        * Test for specific product on Windows NT 4.0 SP5 and earlier
        */
       HKEY hKey;
       char szProductType[BUFSIZE];
       DWORD dwBufLen=BUFSIZE;
       LONG lRet;
       lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                           "SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
                           0, KEY_QUERY_VALUE, &hKey);
       if (lRet != ERROR_SUCCESS)
           return;
       lRet = RegQueryValueEx(hKey, "ProductType", NULL, NULL,
                              (LPBYTE) szProductType, &dwBufLen);
       if ((lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE))
           return;
       RegCloseKey(hKey);
       if (lstrcmpi("WINNT", szProductType) == 0)
       {
           if (m_osvi.dwMajorVersion <= 4)
           {
               m_nWinVersion = WindowsNT40;
               m_nWinEdition = Workstation;
           }
       }
       if (lstrcmpi("LANMANNT", szProductType) == 0)
       {
           if (m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 2)
           {
               m_nWinVersion = WindowsServer2003;
           }
           if (m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 0)
           {
               m_nWinVersion = Windows2000;
               m_nWinEdition = Server;
           }
           if (m_osvi.dwMajorVersion <= 4)
           {
               m_nWinVersion = WindowsNT40;
               m_nWinEdition = Server;
           }
       }
       if (lstrcmpi("SERVERNT", szProductType) == 0)
       {
           if (m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 2)
           {
               m_nWinVersion = WindowsServer2003;
               m_nWinEdition = EnterpriseServer;
           }
           if (m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 0)
           {
               m_nWinVersion = Windows2000;
               m_nWinEdition = AdvancedServer;
           }
           if (m_osvi.dwMajorVersion <= 4)
           {
               m_nWinVersion = WindowsNT40;
               m_nWinEdition = EnterpriseServer;
           }
       }
   }
}

void SystemInfo::DetectWindowsEdition()
{
    if (m_bOsVersionInfoEx)
    {
        switch (m_osvi.dwMajorVersion)
        {
          case 4:
            switch(m_osvi.wProductType)
            {
              case VER_NT_WORKSTATION:
                m_nWinEdition = Workstation;
                break;
              case VER_NT_SERVER:
                m_nWinEdition = ((m_osvi.wSuiteMask & VER_SUITE_ENTERPRISE) != 0 ?
                                 EnterpriseServer :
                                 StandardServer);
                break;
            }
            break;
          case 5:
            switch (m_osvi.wProductType)
            {
              case VER_NT_WORKSTATION:
                m_nWinEdition = ((m_osvi.wSuiteMask & VER_SUITE_PERSONAL) != 0 ?
                                 Home :
                                 Professional);
                break;
              case VER_NT_SERVER:
                switch (m_osvi.dwMinorVersion)
                {
                  case 0:
                    if ((m_osvi.wSuiteMask & VER_SUITE_DATACENTER) != 0)
                    {
                        m_nWinEdition = DatacenterServer;
                    }
                    else if ((m_osvi.wSuiteMask & VER_SUITE_ENTERPRISE) != 0)
                    {
                        m_nWinEdition = AdvancedServer;
                    }
                    else
                    {
                        m_nWinEdition = Server;
                    }
                    break;
                  default:
                    if ((m_osvi.wSuiteMask & VER_SUITE_DATACENTER) != 0)
                    {
                        m_nWinEdition = DatacenterServer;
                    }
                    else if ((m_osvi.wSuiteMask & VER_SUITE_ENTERPRISE) != 0)
                    {
                        m_nWinEdition = EnterpriseServer;
                    }
                    else if ((m_osvi.wSuiteMask & VER_SUITE_BLADE) != 0)
                    {
                        m_nWinEdition = WebServer;
                    }
                    else
                    {
                        m_nWinEdition = StandardServer;
                    }
                    break;
                }
                break;
            }
            break;
          case 6:
          case 10:
            DWORD dwReturnedProductType = DetectProductInfo();
            switch (dwReturnedProductType)
            {
              case PRODUCT_UNDEFINED:
                m_nWinEdition = EditionUnknown;
                break;
              case PRODUCT_ULTIMATE:
                m_nWinEdition = Ultimate;
                break;
              case PRODUCT_HOME_BASIC:
                m_nWinEdition = HomeBasic;
                break;
              case PRODUCT_HOME_PREMIUM:
                m_nWinEdition = HomePremium;
                break;
              case PRODUCT_ENTERPRISE:
                m_nWinEdition = Enterprise;
                break;
              case PRODUCT_HOME_BASIC_N:
                m_nWinEdition = HomeBasic_N;
                break;
              case PRODUCT_BUSINESS:
                m_nWinEdition = Business;
                break;
              case PRODUCT_STANDARD_SERVER:
                m_nWinEdition = StandardServer;
                break;
              case PRODUCT_DATACENTER_SERVER:
                m_nWinEdition = DatacenterServer;
                break;
              case PRODUCT_SMALLBUSINESS_SERVER:
                m_nWinEdition = SmallBusinessServer;
                break;
              case PRODUCT_ENTERPRISE_SERVER:
                m_nWinEdition = EnterpriseServer;
                break;
              case PRODUCT_STARTER:
                m_nWinEdition = Starter;
                break;
              case PRODUCT_DATACENTER_SERVER_CORE:
                m_nWinEdition = DatacenterServerCore;
                break;
              case PRODUCT_STANDARD_SERVER_CORE:
                m_nWinEdition = StandardServerCore;
                break;
              case PRODUCT_ENTERPRISE_SERVER_CORE:
                m_nWinEdition = EnterpriseServerCore;
                break;
              case PRODUCT_ENTERPRISE_SERVER_IA64:
                m_nWinEdition = EnterpriseServerIA64;
                break;
              case PRODUCT_BUSINESS_N:
                m_nWinEdition = Business_N;
                break;
              case PRODUCT_WEB_SERVER:
                m_nWinEdition = WebServer;
                break;
              case PRODUCT_CLUSTER_SERVER:
                m_nWinEdition = ClusterServer;
                break;
              case PRODUCT_HOME_SERVER:
                m_nWinEdition = HomeServer;
                break;
              case PRODUCT_STORAGE_EXPRESS_SERVER:
                m_nWinEdition = StorageExpressServer;
                break;
              case PRODUCT_STORAGE_STANDARD_SERVER:
                m_nWinEdition = StorageStandardServer;
                break;
              case PRODUCT_STORAGE_WORKGROUP_SERVER:
                m_nWinEdition = StorageWorkgroupServer;
                break;
              case PRODUCT_STORAGE_ENTERPRISE_SERVER:
                m_nWinEdition = StorageEnterpriseServer;
                break;
              case PRODUCT_SERVER_FOR_SMALLBUSINESS:
                m_nWinEdition = ServerForSmallBusiness;
                break;
              case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
                m_nWinEdition = SmallBusinessServerPremium;
                break;
#if _WIN32_WINNT >= 0x0601 // windows 7
              case PRODUCT_HOME_PREMIUM_N:
                m_nWinEdition = HomePremium_N;
                break;
              case PRODUCT_ENTERPRISE_N:
                m_nWinEdition = Enterprise_N;
                break;
              case PRODUCT_ULTIMATE_N:
                m_nWinEdition = Ultimate_N;
                break;
              case PRODUCT_WEB_SERVER_CORE:
                m_nWinEdition = WebServerCore;
                break;
              case PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT:
                m_nWinEdition = MediumBusinessServerManagement;
                break;
              case PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY:
                m_nWinEdition = MediumBusinessServerSecurity;
                break;
              case PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING:
                m_nWinEdition = MediumBusinessServerMessaging;
                break;
              case PRODUCT_SERVER_FOUNDATION:
                m_nWinEdition = ServerFoundation;
                break;
              case PRODUCT_HOME_PREMIUM_SERVER:
                m_nWinEdition = HomePremiumServer;
                break;
              case PRODUCT_SERVER_FOR_SMALLBUSINESS_V:
                m_nWinEdition = ServerForSmallBusiness_V;
                break;
              case PRODUCT_STANDARD_SERVER_V:
                m_nWinEdition = StandardServer_V;
                break;
              case PRODUCT_DATACENTER_SERVER_V:
                m_nWinEdition = DatacenterServer_V;
                break;
              case PRODUCT_ENTERPRISE_SERVER_V:
                m_nWinEdition = EnterpriseServer_V;
                break;
              case PRODUCT_DATACENTER_SERVER_CORE_V:
                m_nWinEdition = DatacenterServerCore_V;
                break;
              case PRODUCT_STANDARD_SERVER_CORE_V:
                m_nWinEdition = StandardServerCore_V;
                break;
              case PRODUCT_ENTERPRISE_SERVER_CORE_V:
                m_nWinEdition = EnterpriseServerCore_V;
                break;
              case PRODUCT_HYPERV:
                m_nWinEdition = HyperV;
                break;
              case PRODUCT_STORAGE_EXPRESS_SERVER_CORE:
                m_nWinEdition = StorageExpressServerCore;
                break;
              case PRODUCT_STORAGE_STANDARD_SERVER_CORE:
                m_nWinEdition = StorageStandardServerCore;
                break;
              case PRODUCT_STORAGE_WORKGROUP_SERVER_CORE:
                m_nWinEdition = StorageWorkgroupServerCore;
                break;
              case PRODUCT_STORAGE_ENTERPRISE_SERVER_CORE:
                m_nWinEdition = StorageEnterpriseServerCore;
                break;
              case PRODUCT_STARTER_N:
                m_nWinEdition = Starter_N;
                break;
              case PRODUCT_PROFESSIONAL:
                m_nWinEdition = Professional;
                break;
              case PRODUCT_PROFESSIONAL_N:
                m_nWinEdition = Professional_N;
                break;
              case PRODUCT_SB_SOLUTION_SERVER:
                m_nWinEdition = SBSolutionServer;
                break;
              case PRODUCT_SERVER_FOR_SB_SOLUTIONS:
                m_nWinEdition = ServerForSBSolution;
                break;
              case PRODUCT_STANDARD_SERVER_SOLUTIONS:
                m_nWinEdition = StandardServerSolutions;
                break;
              case PRODUCT_STANDARD_SERVER_SOLUTIONS_CORE:
                m_nWinEdition = StandardServerSolutionsCore;
                break;
              case PRODUCT_SB_SOLUTION_SERVER_EM:
                m_nWinEdition = SBSolutionServer_EM;
                break;
              case PRODUCT_SERVER_FOR_SB_SOLUTIONS_EM:
                m_nWinEdition = ServerForSBSolution_EM;
                break;
              case PRODUCT_SOLUTION_EMBEDDEDSERVER:
                m_nWinEdition = SolutionEmbeddedServer;
                break;
              case PRODUCT_SOLUTION_EMBEDDEDSERVER_CORE:
                m_nWinEdition = SolutionEmbeddedServerCore;
                break;
              case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM_CORE:
                m_nWinEdition = SmallBusinessServerPremiumCore;
                break;
              case PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT:
                m_nWinEdition = EssentialBusinessServerMGMT;
                break;
              case PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL:
                m_nWinEdition = EssentialBusinessServerADDL;
                break;
              case PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC:
                m_nWinEdition = EssentialBusinessServerMGMTSVC;
                break;
              case PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC:
                m_nWinEdition = EssentialBusinessServerADDLSVC;
                break;
              case PRODUCT_CLUSTER_SERVER_V:
                m_nWinEdition = ClusterServer_V;
                break;
              case PRODUCT_EMBEDDED:
                m_nWinEdition = Embedded;
                break;
              case PRODUCT_STARTER_E:
                m_nWinEdition = Starter_E;
                break;
              case PRODUCT_HOME_BASIC_E:
                m_nWinEdition = HomeBasic_E;
                break;
              case PRODUCT_HOME_PREMIUM_E:
                m_nWinEdition = HomePremium_E;
                break;
              case PRODUCT_PROFESSIONAL_E:
                m_nWinEdition = Professional_E;
                break;
              case PRODUCT_ENTERPRISE_E:
                m_nWinEdition = Enterprise_E;
                break;
              case PRODUCT_ULTIMATE_E:
                m_nWinEdition = Ultimate_E;
                break;
#endif
#if _WIN32_WINNT >= 0x0602 // windows 8
              case PRODUCT_ENTERPRISE_EVALUATION:
                m_nWinEdition = EnterpriseEvaluation;
                break;
              case PRODUCT_MULTIPOINT_STANDARD_SERVER:
                m_nWinEdition = MultipointStandardServer;
                break;
              case PRODUCT_MULTIPOINT_PREMIUM_SERVER:
                m_nWinEdition = MultipointPremiumServer;
                break;
              case PRODUCT_STANDARD_EVALUATION_SERVER:
                m_nWinEdition = StandardEvaluationServer;
                break;
              case PRODUCT_DATACENTER_EVALUATION_SERVER:
                m_nWinEdition = DatacenterEvaluationServer;
                break;
              case PRODUCT_ENTERPRISE_N_EVALUATION:
                m_nWinEdition = EnterpriseNEvaluation;
                break;
              case PRODUCT_EMBEDDED_AUTOMOTIVE:
                m_nWinEdition = EmbeddedAutomotive;
                break;
              case PRODUCT_EMBEDDED_INDUSTRY_A:
                m_nWinEdition = EmbeddedIndustryA;
                break;
              case PRODUCT_THINPC:
                m_nWinEdition = Thinpc;
                break;
              case PRODUCT_EMBEDDED_A:
                m_nWinEdition = EmbeddedA;
                break;
              case PRODUCT_EMBEDDED_INDUSTRY:
                m_nWinEdition = EmbeddedIndustry;
                break;
              case PRODUCT_EMBEDDED_E:
                m_nWinEdition = EmbeddedE;
                break;
              case PRODUCT_EMBEDDED_INDUSTRY_E:
                m_nWinEdition = EmbeddedIndustryE;
                break;
              case PRODUCT_EMBEDDED_INDUSTRY_A_E:
                m_nWinEdition = EmbeddedIndustryAE;
                break;
              case PRODUCT_STORAGE_WORKGROUP_EVALUATION_SERVER:
                m_nWinEdition = StorageWorkgroupEvaluationServer;
                break;
              case PRODUCT_STORAGE_STANDARD_EVALUATION_SERVER:
                m_nWinEdition = StorageStandardEvaluationServer;
                break;
              case PRODUCT_CORE_ARM:
                m_nWinEdition = CoreArm;
                break;
              case PRODUCT_CORE_N:
                m_nWinEdition = CoreN;
                break;
              case PRODUCT_CORE_COUNTRYSPECIFIC:
                m_nWinEdition = CoreCountrySpecific;
                break;
              case PRODUCT_CORE_SINGLELANGUAGE:
                m_nWinEdition = CoreSingleLanguage;
                break;
              case PRODUCT_CORE:
                m_nWinEdition = Core;
                break;
              case PRODUCT_PROFESSIONAL_WMC:
                m_nWinEdition = ProfessionalWmc;
                break;
              case PRODUCT_MOBILE_CORE:
                m_nWinEdition = MobileCore;
                break;
#endif
#if _WIN32_WINNT >= 0x0603 // windows 8.1, Windows 10, ...
              case PRODUCT_EDUCATION:
                m_nWinEdition = Education;
                break;
              case PRODUCT_EDUCATION_N:
                m_nWinEdition = EducationN;
                break;
              case PRODUCT_ENTERPRISE_S:
                m_nWinEdition = EnterpriseS;
                break;
              case PRODUCT_ENTERPRISE_S_N:
                m_nWinEdition = EnterpriseSN;
                break;
              case PRODUCT_ENTERPRISE_S_EVALUATION:
                m_nWinEdition = EnterpriseSEvaluation;
                break;
              case PRODUCT_ENTERPRISE_S_N_EVALUATION:
                m_nWinEdition = EnterpriseSNEvaluation;
                break;
              case PRODUCT_MOBILE_ENTERPRISE:
                m_nWinEdition = MobileEnterprise;
                break;
#endif
              default:
                fprintf(stderr, "Unknown Product type: %x\n", static_cast<unsigned int>(dwReturnedProductType));
                m_nWinEdition = EditionUnknown;
                break;
            }
            break;
        }
    }
}

void SystemInfo::DetectWindowsServicePack()
{
    /**
     * Display service pack (if any) and build number.
     */
    if ((m_osvi.dwMajorVersion == 4) &&
        (lstrcmpi(m_osvi.szCSDVersion, "Service Pack 6") == 0))
    {
        HKEY hKey;
        LONG lRet;
        /**
         * Test for SP6 versus SP6a.
         */
        lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                            "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009",
                            0, KEY_QUERY_VALUE, &hKey);
        if (lRet == ERROR_SUCCESS)
            strcpy(m_szServicePack, "Service Pack 6a");
        else
        {
            /**
             * Windows NT 4.0 prior to SP6a
             */
            strcpy(m_szServicePack, m_osvi.szCSDVersion);
        }
        RegCloseKey(hKey);
    }
    else
    {
        /**
         * Windows NT 3.51 and earlier or Windows 2000 and later
         */
        strcpy(m_szServicePack, m_osvi.szCSDVersion);
    }
}

void SystemInfo::DetectCurrentVersionKeys()
{
    const char *szPath = "Software\\Microsoft\\Windows NT\\CurrentVersion"; // Path of key
    HKEY hKey = NULL; // Key used to hget value
    DWORD dwSize; // To tell buff size and recieve actual size
    int access = KEY_QUERY_VALUE; // access rights - we want to read
    BYTE DigitalProductId[512];

    if ((m_osvi.dwMajorVersion >= 6) && Is64bitPlatform())
    {
        access |= KEY_WOW64_64KEY;
    }
    // First open a key to allow you to read the registry
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, // Main key to browse
                     szPath, // sub key
                     0,
                     access,
                     &hKey) != ERROR_SUCCESS) // Recieve the key we want to use
    {
        m_dwUBR = -1;
        *m_szReleaseId = '\0';
        *m_szReleaseName = '\0';
        *m_szCSDBuildNumber = '\0';
        *m_szRegisteredOwner = '\0';
        *m_szRegisteredOrganization = '\0';
        *m_szProductId = '\0';
        *m_szEncodedProductId = '\0';
        return;
    }
    /**
     * Get Registered Owner
     */
    dwSize = sizeof(m_szRegisteredOwner);
    if (RegQueryValueEx(hKey, // From previous call
                        "RegisteredOwner", // value we want to look at
                        0,
                        NULL, // not needed,we know its a string
                        (LPBYTE)m_szRegisteredOwner, // Put info here
                        &dwSize) != ERROR_SUCCESS) // How big is the buffer?
    {
        *m_szRegisteredOwner = '\0';
    }
    /**
     * Get Registered Organization
     */
    dwSize = sizeof(m_szRegisteredOwner);
    if (RegQueryValueEx(hKey, // From previous call
                        "RegisteredOrganization", // value we want to look at
                        0,
                        NULL, // not needed,we know its a string
                        (LPBYTE)m_szRegisteredOrganization, // Put info here
                        &dwSize) != ERROR_SUCCESS) // How big is the buffer?
    {
        *m_szRegisteredOrganization = '\0';
    }
    /**
     * Get Product ID
     */
    dwSize = sizeof(m_szProductId);
    if (RegQueryValueEx(hKey, // From previous call
                        "ProductId", // value we want to look at
                        0,
                        NULL, // not needed,we know its a string
                        (LPBYTE)m_szProductId, // Put info here
                        &dwSize) != ERROR_SUCCESS) // How big is the buffer?
    {
        *m_szProductId = '\0';
    }
    /**
     * Get Digital Product ID
     */
    dwSize = sizeof(DigitalProductId);
    if (RegQueryValueEx(hKey, // From previous call
                        "DigitalProductId", // value we want to look at
                        0,
                        NULL, // not needed,we know its a string
                        (LPBYTE)DigitalProductId, // Put info here
                        &dwSize) != ERROR_SUCCESS) // How big is the buffer?
    {
        *m_szEncodedProductId = '\0';
    }
    else
    {
        EncodeProductId(DigitalProductId);
    }
    if (m_osvi.dwMajorVersion >= 10)
    {
        *m_szCSDBuildNumber = '\0';
        /**
         * Get ReleaaseId
         */
        dwSize = sizeof(m_szReleaseId);
        if (RegQueryValueEx(hKey, // From previous call
                            "ReleaseId", // value we want to look at
                            0,
                            NULL, // not needed,we know its a string
                            (LPBYTE)m_szReleaseId, // Put info here
                            &dwSize) != ERROR_SUCCESS) // How big is the buffer?
        {
            strcpy(m_szReleaseId, "1507");
        }
        /**
         * Get Release Name if possible
         */
        if (strcmp(m_szReleaseId, "1507") == 0)
            strcpy(m_szReleaseName, "Initial Update");
        else if (strcmp(m_szReleaseId, "1511") == 0)
            strcpy(m_szReleaseName, "November Update");
        else if (strcmp(m_szReleaseId, "1607") == 0)
            strcpy(m_szReleaseName, "Anniversary Update");
        else if (strcmp(m_szReleaseId, "1703") == 0)
            strcpy(m_szReleaseName, "Creators Update");
        else if (strcmp(m_szReleaseId, "1709") == 0)
            strcpy(m_szReleaseName, "Fall Creators Update");
        else if (strcmp(m_szReleaseId, "1803") == 0)
            strcpy(m_szReleaseName, "April 2018 Update");
        else if (strcmp(m_szReleaseId, "1809") == 0)
            strcpy(m_szReleaseName, "October 2018 Update");
        else if (strcmp(m_szReleaseId, "1903") == 0)
            strcpy(m_szReleaseName, "May 2019 Update");
        else if (strcmp(m_szReleaseId, "1909") == 0)
            strcpy(m_szReleaseName, "November 2019 Update");
        else if (strcmp(m_szReleaseId, "2004") == 0)
            strcpy(m_szReleaseName, "May 2020 Update");
        else if (strcmp(m_szReleaseId, "2009") == 0)
            strcpy(m_szReleaseName, "October 2020 Update");
        /**
         * Get UBR
         */
        dwSize = sizeof(DWORD);
        if (RegQueryValueEx(hKey, // From previous call
                            "UBR", // value we want to look at
                            0,
                            NULL, // not needed,we know its a DWORD
                            (LPBYTE)(&m_dwUBR), // Put info here
                            &dwSize) != ERROR_SUCCESS) // How big is the buffer?
        {
            m_dwUBR = -1;
        }
    }
    else
    {
        *m_szReleaseId = '\0';
        *m_szReleaseName = '\0';
        m_dwUBR = -1;
        /**
         * Get CSD Build Number
         */
        dwSize = sizeof(m_szCSDBuildNumber);
        if (RegQueryValueEx(hKey, // From previous call
                            "CSDBuildNumber", // value we want to look at
                            0,
                            NULL, // not needed,we know its a DWORD
                            (LPBYTE)(&m_szCSDBuildNumber), // Put info here
                            &dwSize) != ERROR_SUCCESS) // How big is the buffer?
        {
            *m_szCSDBuildNumber = '\0';
        }
    }
}

void SystemInfo::EncodeProductId(LPBYTE Key)
{
    const int KeyOffset = 52;
    int i = 29;
    const char *Chars = "BCDFGHJKMPQRTVWXY2346789";

    m_szEncodedProductId[i] = '\0';
    do
    {
        int Cur = 0;
        for (int x = 14; x >= 0; --x)
        {
            Cur *= 256;
            Cur += Key[x + KeyOffset];
            Key[x + KeyOffset] = (Cur / 24) & 255;
            Cur %= 24;
        }
        m_szEncodedProductId[--i] = Chars[Cur];
        if ((i > 0) && (((30 - i) % 6) == 0))
        {
            m_szEncodedProductId[--i] = '-';
        }
    }
    while (i > 0);
}


/**
 *  Mnemonic                                      Value      Meaning
 * PRODUCT_BUSINESS                            0x00000006 Business
 * PRODUCT_BUSINESS_N                          0x00000010 Business N
 * PRODUCT_CLUSTER_SERVER                      0x00000012 HPC Edition
 * PRODUCT_CLUSTER_SERVER_V                    0x00000040 Server Hyper Core V
 * PRODUCT_CORE                                0x00000065 Windows 10 Home
 * PRODUCT_CORE_COUNTRYSPECIFIC                0x00000063 Windows 10 Home China
 * PRODUCT_CORE_N                              0x00000062 Windows 10 Home N
 * PRODUCT_CORE_SINGLELANGUAGE                 0x00000064 Windows 10 Home Single Language
 * PRODUCT_EDUCATION                           0x00000079 Windows 10 Education
 * PRODUCT_EDUCATION_N                         0x0000007A Windows 10 Education N
 * PRODUCT_DATACENTER_EVALUATION_SERVER        0x00000050 Server Datacenter (evaluation installation)
 * PRODUCT_DATACENTER_SERVER                   0x00000008 Server Datacenter (full installation)
 * PRODUCT_DATACENTER_SERVER_CORE              0x0000000C Server Datacenter (core installation)
 * PRODUCT_DATACENTER_SERVER_CORE_V            0x00000027 Server Datacenter without Hyper-V (core installation)
 * PRODUCT_DATACENTER_SERVER_V                 0x00000025 Server Datacenter without Hyper-V (full installation)
 * PRODUCT_ENTERPRISE                          0x00000004 Windows 10 Enterprise
 * PRODUCT_ENTERPRISE_E                        0x00000046 Windows 10 Enterprise E
 * PRODUCT_ENTERPRISE_EVALUATION               0x00000048 Windows 10 Enterprise Evaluation
 * PRODUCT_ENTERPRISE_N                        0x0000001B Windows 10 Enterprise N
 * PRODUCT_ENTERPRISE_N_EVALUATION             0x00000054 Windows 10 Enterprise N Evaluation
 * PRODUCT_ENTERPRISE_S                        0x0000007D Windows 10 Enterprise 2015 LTSB
 * PRODUCT_ENTERPRISE_S_EVALUATION             0x00000081 Windows 10 Enterprise 2015 LTSB Evaluation
 * PRODUCT_ENTERPRISE_S_N                      0x0000007E Windows 10 Enterprise 2015 LTSB N
 * PRODUCT_ENTERPRISE_S_N_EVALUATION           0x00000082 Windows 10 Enterprise 2015 LTSB N Evaluation
 * PRODUCT_ENTERPRISE_SERVER                   0x0000000A Server Enterprise (full installation)
 * PRODUCT_ENTERPRISE_SERVER_CORE              0x0000000E Server Enterprise (core installation)
 * PRODUCT_ENTERPRISE_SERVER_CORE_V            0x00000029 Server Enterprise without Hyper-V (core installation)
 * PRODUCT_ENTERPRISE_SERVER_IA64              0x0000000F Server Enterprise for Itanium-based Systems
 * PRODUCT_ENTERPRISE_SERVER_V                 0x00000026 Server Enterprise without Hyper-V (full installation)
 * PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL       0x0000003C Windows Essential Server Solution Additional
 * PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC    0x0000003E Windows Essential Server Solution Additional SVC
 * PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT       0x0000003B Windows Essential Server Solution Management
 * PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC    0x0000003D Windows Essential Server Solution Management SVC
 * PRODUCT_HOME_BASIC                          0x00000002 Home Basic
 * PRODUCT_HOME_BASIC_E                        0x00000043 Not supported
 * PRODUCT_HOME_BASIC_N                        0x00000005 Home Basic N
 * PRODUCT_HOME_PREMIUM                        0x00000003 Home Premium
 * PRODUCT_HOME_PREMIUM_E                      0x00000044 Not supported
 * PRODUCT_HOME_PREMIUM_N                      0x0000001A Home Premium N
 * PRODUCT_HOME_PREMIUM_SERVER                 0x00000022 Windows Home Server 2011
 * PRODUCT_HOME_SERVER                         0x00000013 Windows Storage Server 2008 R2 Essentials
 * PRODUCT_HYPERV                              0x0000002A Microsoft Hyper-V Server
 * PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT    0x0000001E Windows Essential Business Server Management Server
 * PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING     0x00000020 Windows Essential Business Server Messaging Server
 * PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY      0x0000001F Windows Essential Business Server Security Server
 * PRODUCT_MOBILE_CORE                         0x00000068 Windows 10 Mobile
 * PRODUCT_MOBILE_ENTERPRISE                   0x00000085 Windows 10 Mobile Enterprise
 * PRODUCT_MULTIPOINT_PREMIUM_SERVER           0x0000004D Windows MultiPoint Server Premium (full installation)
 * PRODUCT_MULTIPOINT_STANDARD_SERVER          0x0000004C Windows MultiPoint Server Standard (full installation)
 * PRODUCT_PROFESSIONAL                        0x00000030 Windows 10 Pro
 * PRODUCT_PROFESSIONAL_E                      0x00000045 Not supported
 * PRODUCT_PROFESSIONAL_N                      0x00000031 Windows 10 Pro N
 * PRODUCT_PROFESSIONAL_WMC                    0x00000067 Professional with Media Center
 * PRODUCT_SB_SOLUTION_SERVER                  0x00000032 Windows Small Business Server 2011 Essentials
 * PRODUCT_SB_SOLUTION_SERVER_EM               0x00000036 Server For SB Solutions EM
 * PRODUCT_SERVER_FOR_SB_SOLUTIONS             0x00000033 Server For SB Solutions
 * PRODUCT_SERVER_FOR_SB_SOLUTIONS_EM          0x00000037 Server For SB Solutions EM
 * PRODUCT_SERVER_FOR_SMALLBUSINESS            0x00000018 Windows Server 2008 for Windows Essential Server Solutions
 * PRODUCT_SERVER_FOR_SMALLBUSINESS_V          0x00000023 Windows Server 2008 without Hyper-V
 *                                                                for Windows Essential Server Solutions
 * PRODUCT_SERVER_FOUNDATION                   0x00000021 Server Foundation
 * PRODUCT_SMALLBUSINESS_SERVER                0x00000009 Windows Small Business Server
 * PRODUCT_SMALLBUSINESS_SERVER_PREMIUM        0x00000019 Small Business Server Premium
 * PRODUCT_SMALLBUSINESS_SERVER_PREMIUM_CORE   0x0000003F Small Business Server Premium (core installation)
 * PRODUCT_SOLUTION_EMBEDDEDSERVER             0x00000038 Windows MultiPoint Server
 * PRODUCT_STANDARD_EVALUATION_SERVER          0x0000004F Server Standard (evaluation installation)
 * PRODUCT_STANDARD_SERVER                     0x00000007 Server Standard
 * PRODUCT_STANDARD_SERVER_CORE                0x0000000D Server Standard (core installation)
 * PRODUCT_STANDARD_SERVER_CORE_V              0x00000028 Server Standard without Hyper-V (core installation)
 * PRODUCT_STANDARD_SERVER_V                   0x00000024 Server Standard without Hyper-V
 * PRODUCT_STANDARD_SERVER_SOLUTIONS           0x00000034 Server Solutions Premium
 * PRODUCT_STANDARD_SERVER_SOLUTIONS_CORE      0x00000035 Server Solutions Premium (core installation)
 * PRODUCT_STARTER                             0x0000000B Starter
 * PRODUCT_STARTER_E                           0x00000042 Not supported
 * PRODUCT_STARTER_N                           0x0000002F Starter N
 * PRODUCT_STORAGE_ENTERPRISE_SERVER           0x00000017 Storage Server Enterprise
 * PRODUCT_STORAGE_ENTERPRISE_SERVER_CORE      0x0000002E Storage Server Enterprise (core installation)
 * PRODUCT_STORAGE_EXPRESS_SERVER              0x00000014 Storage Server Express
 * PRODUCT_STORAGE_EXPRESS_SERVER_CORE         0x0000002B Storage Server Express (core installation)
 * PRODUCT_STORAGE_STANDARD_EVALUATION_SERVER  0x00000060 Storage Server Standard (evaluation installation)
 * PRODUCT_STORAGE_STANDARD_SERVER             0x00000015 Storage Server Standard
 * PRODUCT_STORAGE_STANDARD_SERVER_CORE        0x0000002C Storage Server Standard (core installation)
 * PRODUCT_STORAGE_WORKGROUP_EVALUATION_SERVER 0x0000005F Storage Server Workgroup (evaluation installation)
 * PRODUCT_STORAGE_WORKGROUP_SERVER            0x00000016 Storage Server Workgroup
 * PRODUCT_STORAGE_WORKGROUP_SERVER_CORE       0x0000002D Storage Server Workgroup (core installation)
 * PRODUCT_ULTIMATE                            0x00000001 Ultimate
 * PRODUCT_ULTIMATE_E                          0x00000047 Not supported
 * PRODUCT_ULTIMATE_N                          0x0000001C Ultimate N
 * PRODUCT_UNDEFINED                           0x00000000 An unknown product
 * PRODUCT_WEB_SERVER                          0x00000011 Web Server (full installation)
 * PRODUCT_WEB_SERVER_CORE                     0x0000001D Web Server (core installation)
 */
DWORD SystemInfo::DetectProductInfo()
{
    DWORD dwProductInfo = PRODUCT_UNDEFINED;

#if _WIN32_WINNT >= 0x0600
    if (m_osvi.dwMajorVersion >= 6)
    {
        PGetProductInfo lpProducInfo = (PGetProductInfo)GetProcAddress(
            GetModuleHandle(_T("kernel32.dll")), "GetProductInfo");
        if (NULL != lpProducInfo)
        {
            lpProducInfo(m_osvi.dwMajorVersion,
                         m_osvi.dwMinorVersion,
                         m_osvi.wServicePackMajor,
                         m_osvi.wServicePackMinor,
                         &dwProductInfo);
        }
    }
#endif
    return dwProductInfo;
}

WindowsVersion SystemInfo::GetWindowsVersion() const
{
    return m_nWinVersion;
}

const char *SystemInfo::GetWindowsVersionName() const
{
    return WindowsVersionStr[m_nWinVersion];
}

WindowsEdition SystemInfo::GetWindowsEdition() const
{
    return m_nWinEdition;
}

const char *SystemInfo::GetWindowsEditionName() const
{
    return WindowsEditionStr[m_nWinEdition];
}

bool SystemInfo::IsNTPlatform() const
{
    return m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

bool SystemInfo::IsWindowsPlatform() const
{
    return m_osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS;
}

bool SystemInfo::IsWin32sPlatform() const
{
    return m_osvi.dwPlatformId == VER_PLATFORM_WIN32s;
}

DWORD SystemInfo::GetMajorVersion() const
{
    return m_osvi.dwMajorVersion;
}

DWORD SystemInfo::GetMinorVersion() const
{
    return m_osvi.dwMinorVersion;
}

DWORD SystemInfo::GetBuildNumber() const
{
    return m_osvi.dwBuildNumber;
}

DWORD SystemInfo::GetPlatformID() const
{
    return m_osvi.dwPlatformId;
}

const char *SystemInfo::GetServicePackInfo() const
{
    return (m_szServicePack);
}

const char *SystemInfo::GetReleaseId() const
{
    return (m_szReleaseId);
}

const char *SystemInfo::GetReleaseName() const
{
    return (m_szReleaseName);
}

const char *SystemInfo::GetCSDBuildNumber() const
{
    return (m_szCSDBuildNumber);
}

const char *SystemInfo::GetRegisteredOwner() const
{
    return (m_szRegisteredOwner);
}

const char *SystemInfo::GetRegisteredOrganization() const
{
    return (m_szRegisteredOrganization);
}

const char *SystemInfo::GetProductId() const
{
    return (m_szProductId);
}

const char *SystemInfo::GetEncodedProductId() const
{
    return (m_szEncodedProductId);
}

DWORD SystemInfo::GetUBR() const
{
    return (m_dwUBR);
}

bool SystemInfo::Is32bitPlatform() const
{
    return !Is64bitPlatform();
}

bool SystemInfo::Is64bitPlatform() const
{
    return ((m_SysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) ||
            (m_SysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) ||
            (m_SysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ALPHA64));
}
