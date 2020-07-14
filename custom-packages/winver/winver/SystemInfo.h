/**
 * copyright (c) 2005 - 2010 Marius Bancila
 * http://www.mariusbancila.ro
 * http://www.mariusbancila.ro/blog
 * http://www.codexpert.ro
 * http://www.codeguru.com
 * http://www.sharparena.com
 */

/**
 * SystemInfo.h: interface for the SystemInfo class.
 */

#include <Windows.h>

#if (defined(__MINGW32__) || defined(__CYGWIN__))
#ifndef PRODUCT_EDUCATION
#define PRODUCT_EDUCATION                           0x00000079
#endif
#ifndef PRODUCT_EDUCATION_N
#define PRODUCT_EDUCATION_N                         0x0000007A
#endif
#ifndef PRODUCT_ENTERPRISE_S
#define PRODUCT_ENTERPRISE_S                        0x0000007D
#endif
#ifndef PRODUCT_ENTERPRISE_S_N
#define PRODUCT_ENTERPRISE_S_N                      0x0000007E
#endif
#ifndef PRODUCT_ENTERPRISE_S_EVALUATION
#define PRODUCT_ENTERPRISE_S_EVALUATION             0x00000081
#endif
#ifndef PRODUCT_ENTERPRISE_S_N_EVALUATION
#define PRODUCT_ENTERPRISE_S_N_EVALUATION           0x00000082
#endif
#ifndef PRODUCT_MOBILE_ENTERPRISE
#define PRODUCT_MOBILE_ENTERPRISE                   0x00000085
#endif
#endif

/**
 * Known Windows versions
 */
enum WindowsVersion {
    Windows, /**< Pre-Windows 95 version */
    Windows32s, /**< Win32s API */
    Windows95, /**< Windows 95 (oiginal) */
    Windows95OSR2, /**< Windows 95 0SR2 */
    Windows98, /**< Windows 98 */
    Windows98SE, /**< Windows 98 Second Edition */
    WindowsMillennium, /**< Windows Milenium */
    WindowsNT351, /**< Windows NT 4.51 */
    WindowsNT40, /**< Windows NT 4.0 WorkStation */
    WindowsNT40Server, /**< Windows NT 4.0 Server */
    Windows2000, /**< Windows 2000 */
    WindowsXP, /**< Windows XP */
    WindowsXPProfessionalx64, /**< Windows XP Professionnal X64 */
    WindowsHomeServer, /**< Windows Home Server */
    WindowsServer2003, /**< Windows 2003 Server */
    WindowsServer2003R2, /**< Windows 2003 Server R2 */
    WindowsVista, /**< Windows Vista */
    WindowsServer2008, /**< Windows 2008 Server */
    Windows7, /**< Windows 7 */
    WindowsServer2008R2, /**< Windows 2008 Server R2 */
    Windows8, /**< Windows 8 */
    WindowsServer2012, /**< Windows 2012 Server */
    Windows8_1, /**< Windows 8.1 */
    WindowsServer2012R2, /**< Windows 2008 Server R2 */
    Windows10, /**< Windows 10 */
    WindowsServer2016 /**< Windows 2016 Server */
};

/**
 * Known Windows Editions
 */
enum WindowsEdition {
   EditionUnknown,

   Workstation,
   Server,
   AdvancedServer,
   Home,

   Ultimate,
   HomeBasic,
   HomePremium,
   Enterprise,
   HomeBasic_N,
   Business,
   StandardServer,
   DatacenterServer,
   SmallBusinessServer,
   EnterpriseServer,
   Starter,
   DatacenterServerCore,
   StandardServerCore,
   EnterpriseServerCore,
   EnterpriseServerIA64,
   Business_N,
   WebServer,
   ClusterServer,
   HomeServer,
   StorageExpressServer,
   StorageStandardServer,
   StorageWorkgroupServer,
   StorageEnterpriseServer,
   ServerForSmallBusiness,
   SmallBusinessServerPremium,
   HomePremium_N,
   Enterprise_N,
   Ultimate_N,
   WebServerCore,
   MediumBusinessServerManagement,
   MediumBusinessServerSecurity,
   MediumBusinessServerMessaging,
   ServerFoundation,
   HomePremiumServer,
   ServerForSmallBusiness_V,
   StandardServer_V,
   DatacenterServer_V,
   EnterpriseServer_V,
   DatacenterServerCore_V,
   StandardServerCore_V,
   EnterpriseServerCore_V,
   HyperV,
   StorageExpressServerCore,
   StorageStandardServerCore,
   StorageWorkgroupServerCore,
   StorageEnterpriseServerCore,
   Starter_N,
   Professional,
   Professional_N,
   SBSolutionServer,
   ServerForSBSolution,
   StandardServerSolutions,
   StandardServerSolutionsCore,
   SBSolutionServer_EM,
   ServerForSBSolution_EM,
   SolutionEmbeddedServer,
   SolutionEmbeddedServerCore,
   SmallBusinessServerPremiumCore,
   EssentialBusinessServerMGMT,
   EssentialBusinessServerADDL,
   EssentialBusinessServerMGMTSVC,
   EssentialBusinessServerADDLSVC,
   ClusterServer_V,
   Embedded,
   Starter_E,
   HomeBasic_E,
   HomePremium_E,
   Professional_E,
   Enterprise_E,
   Ultimate_E,
   EnterpriseEvaluation,
   MultipointStandardServer,
   MultipointPremiumServer,
   StandardEvaluationServer,
   DatacenterEvaluationServer,
   EnterpriseNEvaluation,
   EmbeddedAutomotive,
   EmbeddedIndustryA,
   Thinpc,
   EmbeddedA,
   EmbeddedIndustry,
   EmbeddedE,
   EmbeddedIndustryE,
   EmbeddedIndustryAE,
   StorageWorkgroupEvaluationServer,
   StorageStandardEvaluationServer,
   CoreArm,
   CoreN,
   CoreCountrySpecific,
   CoreSingleLanguage,
   Core,
   ProfessionalWmc,
   MobileCore,
   Education,
   EducationN,
   EnterpriseS,
   EnterpriseSN,
   EnterpriseSEvaluation,
   EnterpriseSNEvaluation,
   MobileEnterprise
};

class SystemInfo
{
    WindowsVersion	m_nWinVersion;
    WindowsEdition	m_nWinEdition;
    TCHAR		m_szServicePack[32];
    TCHAR		m_szReleaseId[5];
    TCHAR		m_szReleaseName[128];
    TCHAR		m_szCSDBuildNumber[6];
    TCHAR		m_szRegisteredOwner[128];
    TCHAR		m_szRegisteredOrganization[128];
    TCHAR		m_szProductId[32];
    TCHAR		m_szEncodedProductId[30];
    DWORD		m_dwUBR;
    OSVERSIONINFOEX     m_osvi;
    SYSTEM_INFO		m_SysInfo;
    BOOL		m_bOsVersionInfoEx;
  private:
    void DetectWindowsVersion();
    void DetectWindowsEdition();
    void DetectWindowsServicePack();
    void DetectCurrentVersionKeys();
    DWORD DetectProductInfo();
    void EncodeProductId(LPBYTE lpbDigitalProductId);
  public:
    SystemInfo();
    virtual ~SystemInfo();
    /**
     * returns the windows version (mnemonic)
     */
    WindowsVersion GetWindowsVersion() const;
    /**
     * returns the windows version (string)
     */
    const char *GetWindowsVersionName() const;
    /**
     * returns the windows edition (mnemonic)
     */
    WindowsEdition GetWindowsEdition() const;
    /**
     * returns the windows edition (string)
     */
    const char *GetWindowsEditionName() const;
    /**
     * true if NT platform
     */
    bool IsNTPlatform() const;
    /**
     * true is Windows platform
     */
    bool IsWindowsPlatform() const;
    /**
     * true is Win32s platform
     */
    bool IsWin32sPlatform() const;
    /**
     * returns major version
     */
    DWORD GetMajorVersion() const;
    /**
     * returns minor version
     */
    DWORD GetMinorVersion() const;
    /**
     * returns build number
     */
    DWORD GetBuildNumber() const;
    /**
     * returns platform ID
     */
    DWORD GetPlatformID() const;
    /**
     * additional information about service pack
     */
    const char *GetServicePackInfo() const;
    /**
     * Returns Windows 10 Release ID
     */
    const char *GetReleaseId() const;
    /**
     * Returns Windows 10 Release Name if known
     */
    const char *GetReleaseName() const;
    /**
     * Returns CSD Build Number
     */
    const char *GetCSDBuildNumber() const;
    /**
     * Returns Registered Owner
     */
    const char *GetRegisteredOwner() const;
    /**
     * Returns Registered Organization
     */
    const char *GetRegisteredOrganization() const;
    /**
     * Returns Product ID
     */
    const char *GetProductId() const;
    /**
     * Returns Encoded Product ID
     */
    const char *GetEncodedProductId() const;
    /**
     * Teturns CSDBuildNumber or Windows 10 UBR
     */
    DWORD GetUBR() const;
    /**
     * true if platform is 32-bit
     */
    bool Is32bitPlatform() const;
    /**
     * true if platform is 64-bit
     */
    bool Is64bitPlatform() const;
};
