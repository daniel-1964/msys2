/**
 * winver.cpp : Defines the entry point for the console application.
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

#include "SystemInfo.h"

#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <libgen.h>

void DisplayBrief(const SystemInfo &sysInfo)
{
    std::cout << sysInfo.GetWindowsVersionName();
    if (!sysInfo.GetReleaseName().empty())
    {
        std::cout << " " << sysInfo.GetReleaseName();
    }
    else if (!sysInfo.GetReleaseId().empty())
    {
        std::cout << " " << sysInfo.GetReleaseId();
    }
    else if (!sysInfo.GetCSDBuildNumber().empty())
    {
        std::cout << " " << sysInfo.GetCSDBuildNumber();
    }
    std::cout << " " << sysInfo.GetWindowsEditionName() << " Edition";
    if (!sysInfo.GetServicePackInfo().empty())
    {
        std::cout << " " << sysInfo.GetServicePackInfo();
    }
    std::cout << " Version " << sysInfo.GetMajorVersion() << "." << sysInfo.GetMinorVersion() << "."
              << sysInfo.GetBuildNumber();
    if (!sysInfo.GetCSDBuildNumber().empty())
    {
        std::cout << "." << sysInfo.GetCSDBuildNumber();
    }
    if (static_cast<int>(sysInfo.GetUBR()) != -1)
    {
        std::cout << "." << sysInfo.GetUBR();
    }
    std::cout << std::endl;
}

void DisplayLong(const SystemInfo &sysInfo)
{
    std::cout << "OS Name:\t\t " << sysInfo.GetWindowsVersionName();
    if (!sysInfo.GetReleaseName().empty())
    {
        std::cout << " " << sysInfo.GetReleaseName();
    }
    std::cout << std::endl;
    if (!sysInfo.GetReleaseId().empty())
    {
        std::cout << "Release ID:\t\t " << sysInfo.GetReleaseId() << std::endl;
    }
    if (!sysInfo.GetReleaseDate().empty())
    {
        std::cout << "Release Date:\t\t " << sysInfo.GetReleaseDate() << std::endl;
    }
    if (!sysInfo.GetEolDate().empty())
    {
        std::cout << "End of Support:\t\t " << sysInfo.GetEolDate() << std::endl;
    }
    if (!sysInfo.GetCodeName().empty())
    {
        std::cout << "Code Name:\t\t " << sysInfo.GetCodeName() << std::endl;
    }
    std::cout << "OS Edition:\t\t " << sysInfo.GetWindowsEditionName() << " Edition " << std::endl;
    std::cout << "Service Pack:\t\t " << (!sysInfo.GetServicePackInfo().empty() ? sysInfo.GetServicePackInfo() : "none")
              << std::endl;
    std::cout << "Major version:\t\t " << sysInfo.GetMajorVersion() << std::endl;
    std::cout << "Minor version:\t\t " << sysInfo.GetMinorVersion() << std::endl;
    std::cout << "Build number:\t\t " << sysInfo.GetBuildNumber() << std::endl;
    if (!sysInfo.GetCSDBuildNumber().empty())
    {
        std::cout << "CSD Build Number:\t " << sysInfo.GetCSDBuildNumber() << std::endl;
    }
    if (static_cast<int>(sysInfo.GetUBR()) != -1)
    {
        std::cout << "Update Build Revision:\t " << sysInfo.GetUBR() << std::endl;
    }
    std::cout << "Platform type:\t\t ";
    if (sysInfo.IsNTPlatform())
    {
        std::cout << "NT" << std::endl;
    }
    else if (sysInfo.IsWindowsPlatform())
    {
        std::cout << "Windows" << std::endl;
    }
    else if (sysInfo.IsWin32sPlatform())
    {
        std::cout << "Win32s" << std::endl;
    }
    else
    {
        std::cout << "Unknown" << std::endl;
    }
    std::cout << "Architecture:\t\t " << (sysInfo.Is64bitPlatform() ? "64" : "32") << " bits Platform" << std::endl;
    if (!sysInfo.GetProductId().empty())
    {
        std::cout << "Product ID:\t\t " << sysInfo.GetProductId();
        if (!sysInfo.GetEncodedProductId().empty())
        {
            std::cout << " (" << sysInfo.GetEncodedProductId() << ")";
        }
        std::cout << std::endl;
    }
    if (!sysInfo.GetRegisteredOwner().empty())
    {
        std::cout << "Registered Owner:\t " << sysInfo.GetRegisteredOwner() << std::endl;
    }
    if (!sysInfo.GetRegisteredOrganization().empty())
    {
        std::cout << "Registered Organization: " << sysInfo.GetRegisteredOrganization() << std::endl;
    }
}

#if __cplusplus >= 201103L
void ShowBrief(const SystemInfo &sysInfo, const std::string &name, bool reverse)
{
    for (auto &it : sysInfo.Find(name, reverse))
    {
        std::cout << std::left << it.get().m_szReleaseDate << " " << std::setw(16) << it.get().m_Version
                  << it.get().FullName() << std::endl;
    }
}

void ShowLong(const SystemInfo &sysInfo, const std::string &name, bool reverse)
{
    for (auto &it : sysInfo.Find(name, reverse))
    {
        std::cout << "OS Name:\t\t " << it.get().FullName() << std::endl;
        if (!it.get().m_szCodeName.empty())
        {
            std::cout << "Code Name:\t\t " << it.get().m_szCodeName << std::endl;
        }
        std::cout << "Release Date:\t\t " << it.get().m_szReleaseDate << std::endl;
        std::cout << "End of Support:\t\t " << it.get().m_szEolDate << std::endl;
        std::cout << "Major version:\t\t " << static_cast<WORD>(it.get().m_Version.m_nMajor) << std::endl;
        std::cout << "Minor version:\t\t " << static_cast<WORD>(it.get().m_Version.m_nMinor) << std::endl;
        if (it.get().m_Version.m_wBuild != 0)
        {
        std::cout << "Build number:\t\t " << it.get().m_Version.m_wBuild << std::endl;
        }
        std::cout << "Platform type:\t\t "
                  << (it.get().m_Version.m_bIsServer ? "NT Server"
                                                     : (it.get().m_Version.m_bIsNT ? "NT Workstation" : "MS-Dos"))
                  << std::endl;
        std::string prefix("OS Edition:\t\t ");
        for (auto &i : it.get().m_Editions)
        {
            std::cout << prefix << i << std::endl;
            prefix = "\t\t\t ";
        }
        std::cout << std::endl;
    }
}
#endif

void Usage(const std::string &name, const std::string &prefix)
{
    if (!prefix.empty())
    {
        std::cerr << prefix << std::endl << std::endl;
    }
    std::cerr << "Usage " << name << " -bhlsv" << std::endl;
    std::cerr << "\t-b, --brief:\t\tPrint one line Windows version informations" << std::endl;
    std::cerr << "\t-l, --long:\t\tPrint Detailled Windows version informations" << std::endl;
    std::cerr << "\t-h, --help:\t\tPrint this above message" << std::endl;
#if __cplusplus >= 201103L
    std::cerr << "\t-r, --reverse:\tPrint version informations in reverse order" << std::endl;
    std::cerr << "\t-s, --show <name>:\tPrint informations about a specific Windows Version" << std::endl;
    std::cerr << "\t\t\t\tIf no <name> is specified informations about all known Windows versions are displayed"
              << std::endl;
    std::cout << "\t\t\t\tIf more than one Windows version is found, the display is sorted by dates" << std::endl;
#endif
    std::cerr << "\t-v, --version:\t\tPrint the version of this program" << std::endl;
}

void version(const std::string &name)
{
    std::cout << name << " Version 2.0" << std::endl << std::endl;
    std::cout << "Portion Copyright <C> 2005 - 2010 Marius Bancila" << std::endl;
    std::cout << "http://www.mariusbancila.ro" << std::endl;
    std::cout << "http://www.mariusbancila.ro/blog" << std::endl;
    std::cout << "From http://www.codexpert.ro" << std::endl;
    std::cout << "From http://www.codeguru.com" << std::endl;
    std::cout << "From http://www.sharparena.com" << std::endl << std::endl;
    std::cout << "Copyright <C> 2018 - 2023 Various WEB Contributors" << std::endl;
    std::cout << "From https://en.wikipedia.org/wiki/List_of_Microsoft_Windows_versions" << std::endl;
    std::cout << "from https://www.gaijin.at/en/infos/windows-version-numbers" << std::endl;
}

int main(int argc, char *argv[])
{
    SystemInfo sysInfo;
    struct option opts[] = {{"brief", no_argument, NULL, 'b'},      {"help", no_argument, NULL, 'h'},
                            {"long", no_argument, NULL, 'l'},       {"reverse", no_argument, NULL, 'r'},
                            {"show", required_argument, NULL, 's'}, {"version", no_argument, NULL, 'v'},
                            {NULL, no_argument, NULL, '\0'}};
    char c;
    bool brief = true;
#if __cplusplus >= 201103L
    bool show = false;
    bool reverse = false;
    std::string name;
#endif

    while ((c = getopt_long(argc, argv, ":bhlrs:v", opts, NULL)) != -1)
    {
        switch (c)
        {
        case 'b':
            brief = true;
            break;
        case 'l':
            brief = false;
            break;
        case 'v':
            version(basename(argv[0]));
            return 0;
        case 'h':
            Usage(basename(argv[0]), "Print version informations about the currently running Windows Operating System");
            return 0;
#if __cplusplus >= 201103L
        case 'r':
            reverse = true;
            break;
        case 's':
            show = true;
            name = (optarg != NULL) ? optarg : "";
            break;
        case ':':
            if (optopt == 's')
            {
                show = true;
                break;
            }
            else
            {
                Usage(basename(argv[0]),
                      std::string(basename(argv[0])) + ": Missing argument for option -" + static_cast<char>(optopt));
                return 1;
            }
#endif
        default:
            optopt = c;
            /* fallthrough */
        case '?':
            Usage(basename(argv[0]),
                  std::string(basename(argv[0])) + ": Invalid command-line option -" + static_cast<char>(optopt));
            return 1;
        }
    }
    if (optind != argc)
    {
        Usage(basename(argv[0]), std::string(basename(argv[0])) + std::string(": too many arguments"));
        return 1;
    }
#if __cplusplus >= 201103l
    if (show)
    {
        if (brief)
        {
            ShowBrief(sysInfo, name, reverse);
        }
        else
        {
            ShowLong(sysInfo, name, reverse);
        }
        return 0;
    }
#endif
    if (brief)
    {
        DisplayBrief(sysInfo);
    }
    else
    {
        DisplayLong(sysInfo);
    }
    return 0;
}
