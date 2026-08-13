BEGIN {
    inquote = 0;
    title = "";
    author = "";
}

{
    if (inquote)
    {
        if ($0 ~ "^$")
        {
            if (title !~ "^$")
            {
                print "\n\t-+-", author " --> " title, "-+-\n%";
            }
            else
            {
                print "\n\t-+-", author, "-+-\n%";
            }
            inquote = 0;
            author = "";
            title = "";
        }
        else
        {
            print;
        }
    }
    else if (author ~ "^$")
    {
        author = $0;
    }
    else if (title ~ "^$")
    {
        title = $0;
        inquote = 1;
    }
}
