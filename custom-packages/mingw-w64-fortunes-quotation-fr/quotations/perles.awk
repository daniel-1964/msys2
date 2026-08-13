BEGIN {
    incitation = 0;
    matiere = "";
}

{
    if (incitation)
    {
        if ($0 ~ "^Ajouté le")
        {
            incitation = 0;
            print "%";
        }
        else if ($0 !~ "^$")
        {
	    print;
        }
    }
    else if ($0 ~ "^#[0-9]+")
    {
        matiere = $0
        sub(/^#[0-9]+ */, "", matiere);
        printf("%s\n\n", matiere);
        incitation = 1;
    }
}
