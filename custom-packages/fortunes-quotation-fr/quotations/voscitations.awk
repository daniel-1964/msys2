BEGIN {
    incitation = 0;
    author = "";
}

{
    if (incitation)
    {
        if (($0 ~ "^ *Note :") || ($0 ~ "^ *<<") || ($0 ~ ">> *$"))
        {
            incitation = 0;
            printf("\n\t-+-  %s  -+-\n%\n", author);
        }
        else if ($0 !~ "^$")
            print;
    }
    else
    {
        if ($0 ~ "^#[0-9]+")
        {
            author = $0
            sub(/^#[0-9]+ */, "", author);
            incitation = 1;
        }
    }
}
