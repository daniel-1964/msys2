BEGIN {
    header = "";
    footer = "";
    inquote = 0;
    startnote = 0;
}

{
    if (startnote && ($0 ~ /^ *\*/))
    {
        printf("\n%s\n", $0);
        next;
    }
    else if (startnote && (substr($0, 1, 1) == "\""))
    {
        printf("%s\n", $0);
        next;
    }
    else if (startnote)
    {
        startnote = 0;
        printf("%\n");
    }
    if ($0 ~ /^ +\* .+\//)
    {
        sub(/^ +\*/, "", $0);
        header = $0;
        if (!match(header, /^ ([^\/]+) \/ ?([^\/]+)( \/ .*)?$/, A))
        {
            printf("Error: header=%s, footer=%s\n", header, footer);
        }
        author = A[1];
        title = A[2];
        inquote = 1;
        nowriten = 1;
    }
    else if (inquote)
    {
        if (($0 ~ /^$/) && endquote)
        {
            if (nowriten)
            {
                printf("Error: header=%s, footer=%s\n", header, footer);
                nowriten = 0;
            }
            header = "";
            footer = "";
            author = "";
            title = "";
            inquote = 0;
        }
        else if (($0 ~ /^<.*>$/) && endquote)
        {
            footer = $0;
            printf("\n\t-+- ");
            if (author != "***")
            {
                printf("%s --> ", author);
            }
            printf("%s", title);
            if (footer != "<>")
            {
                printf(" %s", footer);
            }
            printf(" -+-\n");
            nowriten = 0;
            startnote = 1;
        }
        else if ($0 !~ /^ +_+$/)
        {
            if ($0 ~ /«/)
            {
                endquote = 0;
                sub(/^.*« */, "", $0);
            }
            if ($0 ~ /»/)
            {
                endquote = 1;
                sub(/ *».*$/, "", $0);
            }
            printf("%s\n", $0);
        }
    }
}
