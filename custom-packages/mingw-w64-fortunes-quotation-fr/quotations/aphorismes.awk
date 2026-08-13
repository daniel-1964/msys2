BEGIN {
    incitation = 0;
    incategory = 0;
}

{
    if (($0 ~ /Older Entries/) ||
        ($0 ~ /Newer Entries/))
    {
        exit;
    }
    else if (incitation)
    {
        if ($0 ~ /^$/)
        {
            incitation = 0;
            print "%";
        }
        else if (match($0, /^(.+) \((.+)\)$/, A))
        {
            quotation = A[1];
            author =  A[2];
	    printf("%s\n\t-+- %s -+-\n", quotation, author);
        }
        else
        {
	    print;
        }
    }
    else if (incategory && ($0 ~ "^$"))
    {
        incitation = 1;
        incategory = 0;
    }
    else if (($0 ~ /^ +\* Catégories /) ||
             ($0 ~ /^Archive /))
    {
        incategory = 1;
    }
}
