# SPICE has an annoying limitation that if a directory path is > 80
# characters we need to split this between multiple lines in a kernel file.
# This awk expression does this

{
    chunk=70;
    x = $0;
    for (i = 1; i < length(x); i+= chunk) {
	printf "   '" substr(x, i, chunk);
	if(length(x) > i+chunk)
	    printf "+',\n";
	else printf "'";
    }
}
