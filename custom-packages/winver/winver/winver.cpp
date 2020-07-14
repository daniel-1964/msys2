// SystemVersion.cpp : Defines the entry point for the console application.
//

#include <getopt.h>
#include <cstdio>
#include <libgen.h>
#include "SystemInfo.h"

int main(int argc, char* argv[])
{
    SystemInfo sysInfo;
    const char *ServicePack = sysInfo.GetServicePackInfo();
    const char *ReleaseId = sysInfo.GetReleaseId();
    const char *ReleaseName = sysInfo.GetReleaseName();
    const char *CSDBuildNumber = sysInfo.GetCSDBuildNumber();
    const char *RegisteredOwner = sysInfo.GetRegisteredOwner();
    const char *RegisteredOrganization = sysInfo.GetRegisteredOrganization();
    const char *ProductId = sysInfo.GetProductId();
    const char *EncodedProductId = sysInfo.GetEncodedProductId();
    int ubr = sysInfo.GetUBR();
    struct option opts[] = {
        { "brief", no_argument, NULL, 'b' },
        { "help", no_argument, NULL, 'h' },
        { "long", no_argument, NULL, 'l' },
        { NULL, no_argument, NULL, '\0' }
    };
    char c;
    bool brief = true;

    while ((c = getopt_long(argc, argv, "bhl", opts, NULL)) != -1)
    {
        switch (c)
        {
          case 'b':
            brief = true;
            break;
          case 'l':
            brief = false;
            break;
          case 'h':
            fprintf(stderr, "Print version informations about the currently running Windows Operating System\n");
            /* fall through */
          default:
          usage:
            fprintf(stderr, "Usage %s -bhl\n", basename(argv[0]));
            fprintf(stderr, "\t-b, --brief: Print one line Windows version informations\n");
            fprintf(stderr, "\t-l, --long:  Printf Detailled Windows version informations\n");
            fprintf(stderr, "\t-h, --help:  Print this above message\n");
            return (0);
        }
    }
    if (optind != argc)
    {
        fprintf(stderr, "%s: too many arguments\n", basename(argv[0]));
        goto usage;
    }
    if (brief)
    {
        printf("%s",
               sysInfo.GetWindowsVersionName());
        if (*ReleaseName)
            printf(" %s", ReleaseName);
        else if (*ReleaseId)
            printf(" %s", ReleaseId);
        printf(" %s Edition",
               sysInfo.GetWindowsEditionName());
        if (*ServicePack)
            printf(" %s", ServicePack);
        printf(" Version %u.%u.%u",
               static_cast<unsigned int>(sysInfo.GetMajorVersion() & 0XFFFF),
               static_cast<unsigned int>(sysInfo.GetMinorVersion() & 0XFFFF),
               static_cast<unsigned int>(sysInfo.GetBuildNumber() & 0xFFFF));
        if (*CSDBuildNumber)
            printf(".%s", CSDBuildNumber);
        if (ubr != -1)
            printf(".%u", static_cast<unsigned int>(ubr & 0xFFFF));
        printf("\n");
    }
    else
    {
        printf("OS Name:\t\t %s\n", sysInfo.GetWindowsVersionName());
        if (*ReleaseId)
        {
            printf("Release ID:\t\t %s", ReleaseId);
            if (*ReleaseName)
                printf(" (%s)", ReleaseName);
            printf("\n");
        }
        printf("OS Edition:\t\t %s Edition\n", sysInfo.GetWindowsEditionName());
        printf("Service Pack:\t\t %s\n", *ServicePack ? ServicePack : "none");
        printf("Major version:\t\t %u\n", static_cast<unsigned int>(sysInfo.GetMajorVersion() & 0xFFFF));
        printf("Minor version:\t\t %u\n", static_cast<unsigned int>(sysInfo.GetMinorVersion() & 0xFFFF));
        printf("Build number:\t\t %u\n", static_cast<unsigned int>(sysInfo.GetBuildNumber() & 0xFFFF));
        if (*CSDBuildNumber)
            printf("CSD Build Number:\t %s\n", CSDBuildNumber);
        if (ubr != -1)
            printf("Update Build Revision:\t %u\n", static_cast<unsigned int>(ubr & 0xFFFF));
        printf("Platform type:\t\t ");
        if (sysInfo.IsNTPlatform())
            printf("NT\n");
        else if (sysInfo.IsWindowsPlatform())
            printf("Windows\n");
        else if (sysInfo.IsWin32sPlatform())
            printf("Win32s\n");
        else
            printf("Unknown\n");
        printf("Architecture:\t\t %s bits Platform\n", sysInfo.Is64bitPlatform() ? "64" : "32");
        if (*ProductId)
        {
            printf("Product ID:\t\t %s", ProductId);
            if (*EncodedProductId)
                printf(" (%s)\n", EncodedProductId);
            else
                printf("\n");
        }
        if (*RegisteredOwner)
            printf("Registered Owner:\t %s\n", RegisteredOwner);
        if (*RegisteredOrganization)
            printf("Registered Organization: %s\n", RegisteredOrganization);
    }
    return 0;
}
