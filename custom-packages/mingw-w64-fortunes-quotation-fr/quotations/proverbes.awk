BEGIN {
    inquote = 0;
    endquote = 0;
    if (count == "")
    {
        count = 1;
    }
    inpicture = 0;
    endpicture = 0;
    hasquote = 0;
}

{
    gsub(/^[[:space:]]+|[[:space:]]$/, "", $0);
    if (inquote)
    {
        if ($0 == "")
        {
            if (hasquote)
            {
                endquote++;
                if (endquote == count)
                {
                    print "\n\t-+- ", type, " -+-\n%";
                    endquote = 0;
                    hasquote = 0;
                }
                else
                {
                    print;
                }
            }
        }
        else if ($0 == stop)
        {
            inquote = 0;
        }
        else if ((length($0) > 1) &&
                 ($0 !~ "^_+$") &&
                 ($0 != "Retour au début"))
        {
            print;
            hasquote = 1;
        }
    }
    else if ($0 == start)
    {
        if (haspicture)
            inpicture = 1;
        else
            inquote = 1;
    }
    else if (inpicture)
    {
        if ($0 != "")
        {
            endpicture = 1;
        }
        else if (endpicture)
        {
            endpicture = 0;
            inpicture = 0;
            inquote = 1;
        }
    }
}
