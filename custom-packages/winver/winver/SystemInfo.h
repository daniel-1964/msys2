/**
 * SystemInfo.h: interface for the SystemInfo class.
 */

/**
 * Portion copyright (C) 2005 - 2010 Marius Bancila
 * http://www.mariusbancila.ro
 * http://www.mariusbancila.ro/blog
 * http://www.codexpert.ro
 * http://www.codeguru.com
 * http://www.sharparena.com
 *
 * copyright <C) 2018 - 2023 Various WEB contributors
 */

#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include <Windows.h>
#include <cstdio>
#include <functional>
#include <list>
#include <string>

/**
 * Known Windows versions
 */
enum WindowsVersion
{
    Windows,                  /**< Pre-Windows 95 version */
    Windows32s,               /**< Win32s API */
    Windows95,                /**< Windows 95 (oiginal) */
    Windows95OSR1,            /**< Windows 95 0SR1 */
    Windows95OSR2,            /**< Windows 95 0SR2 */
    Windows95OSR25,           /**< Windows 95 0SR2.5 */
    Windows98,                /**< Windows 98 */
    Windows98SE,              /**< Windows 98 Second Edition */
    WindowsMillennium,        /**< Windows Milenium */
    WindowsNT351,             /**< Windows NT 4.51 */
    WindowsNT351Server,       /**< Windows NT 4.51 Server */
    WindowsNT40,              /**< Windows NT 4.0 WorkStation */
    WindowsNT40Server,        /**< Windows NT 4.0 Server */
    Windows2000,              /**< Windows 2000 */
    WindowsXP,                /**< Windows XP */
    WindowsXPProfessionalx64, /**< Windows XP Professionnal X64 */
    WindowsHomeServer,        /**< Windows Home Server */
    WindowsServer2003,        /**< Windows 2003 Server */
    WindowsServer2003R2,      /**< Windows 2003 Server R2 */
    WindowsVista,             /**< Windows Vista */
    WindowsServer2008,        /**< Windows 2008 Server */
    Windows7,                 /**< Windows 7 */
    WindowsServer2008R2,      /**< Windows 2008 Server R2 */
    Windows8,                 /**< Windows 8 */
    WindowsHomeServer2011,    /**< Windows Home Server 2011 */
    WindowsServer2012,        /**< Windows 2012 Server */
    Windows8_1,               /**< Windows 8.1 */
    WindowsServer2012R2,      /**< Windows 2008 Server R2 */
    Windows10,                /**< Windows 10 */
    WindowsServer2016,        /**< Windows 2016 Server */
    WindowsServer2019,        /**< Windows 2019 Server */
    Windows11,                /**< Windows 11 */
    WindowsServer2022,        /**< Windows 2022 Server */
};

/**
 * Known Windows Editions
 */
enum WindowsEdition
{
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

#if __cplusplus >= 201103L
struct WindowsBuildInfo
{
    BYTE m_nMajor;    /**< Major version number */
    BYTE m_nMinor;    /**< Minor version number */
    WORD m_wBuild;    /**< Build number */
    BOOL m_bIsNT;     /**< true if it comes from a NT kernel */
    BOOL m_bIsServer; /**< true if it is a Windows Server */
    BYTE m_nSP;       /**< Service Pack if not in m_wBuild */
    WindowsBuildInfo(BYTE major, BYTE minor, WORD build = 0, bool nt = false, bool server = false, BYTE sp = 0)
        : m_nMajor(major), m_nMinor(minor), m_wBuild(build), m_bIsNT(nt), m_bIsServer(server), m_nSP(sp)
    {
    }
    operator==(const WindowsBuildInfo &o) const
    {
        return ((m_bIsNT == o.m_bIsNT) && (m_bIsServer == o.m_bIsServer) && (m_nMajor == o.m_nMajor) &&
                (m_nMinor == o.m_nMinor) && (m_wBuild == o.m_wBuild) && (m_nSP == o.m_nSP));
    }
    operator<(const WindowsBuildInfo &o) const
    {
        return (
            ((m_nMajor == o.m_nMajor)
                 ? ((m_nMinor == o.m_nMinor)
                        ? ((m_wBuild == o.m_wBuild)
                               ? ((m_nSP == o.m_nSP) ? ((m_bIsNT == o.m_bIsNT)
                                                            ? ((m_bIsServer == o.m_bIsServer) ? false : o.m_bIsServer)
                                                            : o.m_bIsNT)
                                                     : (m_nSP < o.m_nSP))
                               : (m_wBuild < o.m_wBuild))
                        : (m_nMinor < o.m_nMinor))
                 : (m_nMajor < o.m_nMajor)));
    }
    operator>(const WindowsBuildInfo &o) const
    {
        return (((m_nMajor == o.m_nMajor)
                     ? ((m_nMinor == o.m_nMinor)
                            ? ((m_wBuild == o.m_wBuild)
                                   ? ((m_nSP == o.m_nSP) ? ((m_bIsNT == o.m_bIsNT)
                                                                ? ((m_bIsServer == o.m_bIsServer) ? false : m_bIsServer)
                                                                : m_bIsNT)
                                                         : (m_nSP > o.m_nSP))
                                   : (m_wBuild > o.m_wBuild))
                            : (m_nMinor > o.m_nMinor))
                     : (m_nMajor > o.m_nMajor)));
    }
    operator<=(const WindowsBuildInfo &o) const
    {
        return (!operator>(o));
    }
    operator>=(const WindowsBuildInfo &o) const
    {
        return (!operator<(o));
    }
    operator!=(const WindowsBuildInfo &o) const
    {
        return (!operator==(o));
    }
    operator std::string() const
    {
        std::string s = m_bIsServer ? "SRV " : (m_bIsNT ? " NT " : "    ");
        if (m_nMajor < 10)
        {
            s += ' ';
        }
        s += std::to_string(m_nMajor) + '.';
        if (!m_bIsNT && (m_nMinor < 10))
        {
            s += '0';
        }
        s += std::to_string(m_nMinor);
        if (m_wBuild != 0)
        {
            s += '.' + std::to_string(m_wBuild);
        }
        return s;
    }
};

inline std::ostream &operator<<(std::ostream &os, const WindowsBuildInfo &v)
{
    const std::string &s = v;
    os << s;
    return os;
}

struct WindowsVersionInfo
{
    std::string m_szVersionName;       /**< Official know version name */
    std::string m_szReleaseName;       /**< Release name / Service Pack */
    std::string m_szCodeName;          /**< Internal/unofficial version name */
    std::string m_szReleaseDate;       /**< Official date of the release */
    std::string m_szEolDate;           /**< End of suport date */
    std::list<std::string> m_Editions; /**< List of available versions */
    WindowsBuildInfo m_Version;        /**< major.minor.buildnumber */
    const std::string FullName() const
    {
        return m_szVersionName + (m_szReleaseName.empty() ? "" : " " + m_szReleaseName);
    }
    operator==(const WindowsVersionInfo &o) const
    {
        return ((m_szReleaseDate == o.m_szReleaseDate) && (m_Version == o.m_Version) && (FullName() == o.FullName()));
    }
    operator<(const WindowsVersionInfo &o) const
    {
        return ((m_szReleaseDate == o.m_szReleaseDate)
                    ? ((m_Version == o.m_Version) ? (FullName() < o.FullName()) : (m_Version < o.m_Version))
                    : (m_szReleaseDate < o.m_szReleaseDate));
    }
    operator>(const WindowsVersionInfo &o) const
    {
        return ((m_szReleaseDate == o.m_szReleaseDate)
                    ? ((m_Version == o.m_Version) ? (FullName() > o.FullName()) : (m_Version > o.m_Version))
                    : (m_szReleaseDate > o.m_szReleaseDate));
    }
    operator<=(const WindowsVersionInfo &o) const
    {
        return (!operator>(o));
    }
    operator>=(const WindowsVersionInfo &o) const
    {
        return (!operator<(o));
    }
    operator!=(const WindowsVersionInfo &o) const
    {
        return (!operator==(o));
    }
};
#endif

class SystemInfo
{
    bool m_bOsVersionInfoEx;
    bool m_bServer;
    DWORD m_dwPatch;
    DWORD m_dwUBR;
    WindowsEdition m_nWinEdition;
    WindowsVersion m_nWinVersion;
    std::string m_szCodeName;
    std::string m_szCSDBuildNumber;
    std::string m_szEditionName;
    std::string m_szEncodedProductId;
    std::string m_szEolDate;
    std::string m_szProductId;
    std::string m_szRegisteredOrganization;
    std::string m_szRegisteredOwner;
    std::string m_szReleaseDate;
    std::string m_szReleaseId;
    std::string m_szReleaseName;
    std::string m_szServicePack;
    std::string m_szVersionName;
    OSVERSIONINFOEX m_Osvi;
    SYSTEM_INFO m_SysInfo;

  private:
    void DetectCurrentVersionKeys();
    DWORD DetectProductInfo();
    void DetectWindowsEdition();
    void DetectWindowsServicePack();
    void DetectWindowsVersion();
    void EncodeProductId(LPBYTE lpbDigitalProductId);
#if __cplusplus >= 201103L
    void Lookup();
#endif

  public:
    SystemInfo();
    virtual ~SystemInfo()
    {
    }
#if __cplusplus >= 201103L
    /**
     * Find all Windows Versions that match <Name>
     * If there are more than one element, the list is sorted according to the release date.
     * If <reverse> is true the list is sorted in the reverse order.
     */
    using VersionRef = std::reference_wrapper<const WindowsVersionInfo>;
    const std::list<VersionRef> Find(const std::string &name, bool reverse = false) const;
#endif
    /**
     * returns build number
     */
    DWORD GetBuildNumber() const
    {
        return m_Osvi.dwBuildNumber;
    }
    /**
     * Returns Windows Internal Code Name
     */
    const std::string &GetCodeName() const
    {
        return (m_szCodeName);
    }
    /**
     * Returns CSD Build Number
     */
    const std::string &GetCSDBuildNumber() const
    {
        return (m_szCSDBuildNumber);
    }
    /**
     * Returns Encoded Product ID
     */
    const std::string &GetEncodedProductId() const
    {
        return (m_szEncodedProductId);
    }
    /**
     * Returns the End of Support Date
     */
    const std::string &GetEolDate() const
    {
        return (m_szEolDate);
    }
    /**
     * returns major version
     */
    DWORD GetMajorVersion() const
    {
        return m_Osvi.dwMajorVersion;
    }
    /**
     * returns minor version
     */
    DWORD GetMinorVersion() const
    {
        return m_Osvi.dwMinorVersion;
    }
    /**
     * returns platform ID
     */
    DWORD GetPlatformID() const
    {
        return m_Osvi.dwPlatformId;
    }
    /**
     * Returns Product ID
     */
    const std::string &GetProductId() const
    {
        return (m_szProductId);
    }
    /**
     * Returns Registered Organization
     */
    const std::string &GetRegisteredOrganization() const
    {
        return (m_szRegisteredOrganization);
    }
    /**
     * Returns Registered Owner
     */
    const std::string &GetRegisteredOwner() const
    {
        return (m_szRegisteredOwner);
    }
    /**
     * Returns Windows Release Date
     */
    const std::string &GetReleaseDate() const
    {
        return (m_szReleaseDate);
    }
    /**
     * Returns Windows 10 Release ID
     */
    const std::string &GetReleaseId() const
    {
        return (m_szReleaseId);
    }
    /**
     * Returns Windows 10 Release Name if known
     */
    const std::string &GetReleaseName() const
    {
        return (m_szReleaseName);
    }
    /**
     * additional information about service pack
     */
    const std::string &GetServicePackInfo() const
    {
        return (m_szServicePack);
    }
    /**
     * Teturns CSDBuildNumber or Windows 10 UBR
     */
    DWORD GetUBR() const
    {
        return (m_dwUBR);
    }
    /**
     * returns the windows edition (mnemonic)
     */
    WindowsEdition GetWindowsEdition() const
    {
        return m_nWinEdition;
    }
    /**
     * returns the windows edition (string)
     */
    const std::string &GetWindowsEditionName() const
    {
        return m_szEditionName;
    }
    /**
     * returns the windows version (mnemonic)
     */
    WindowsVersion GetWindowsVersion() const
    {
        return m_nWinVersion;
    }
    /**
     * returns the windows version (string)
     */
    const std::string &GetWindowsVersionName() const
    {
        return m_szVersionName;
    }
    /**
     * true if platform is 32-bit
     */
    bool Is32bitPlatform() const
    {
        return !Is64bitPlatform();
    }
    /**
     * true if platform is 64-bit
     */
    bool Is64bitPlatform() const
    {
        return ((m_SysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) ||
                (m_SysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) ||
                (m_SysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ALPHA64));
    }
    /**
     * true if NT platform
     */
    bool IsNTPlatform() const
    {
        return m_Osvi.dwPlatformId == VER_PLATFORM_WIN32_NT;
    }
    /**
     * true is Win32s platform
     */
    bool IsWin32sPlatform() const
    {
        return m_Osvi.dwPlatformId == VER_PLATFORM_WIN32s;
    }
    /**
     * true is Windows platform
     */
    bool IsWindowsPlatform() const
    {
        return m_Osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS;
    }

  private:
#if __cplusplus >= 201103L
    static const std::list<WindowsVersionInfo> WindowsVersions;
#endif
    static const char *WindowsVersionStr[];
    static const char *WindowsEditionStr[];
};

#endif /* SYSTEMINFO_H */
