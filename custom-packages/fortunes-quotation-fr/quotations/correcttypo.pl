#!/usr/bin/perl

# Apply regexes in order to make a good French typography
use utf8;
use strict;
use warnings;
use open IO => ':locale';
#use open ":encoding(utf8)";
#use open IN => ":encoding(utf8)", OUT => ":utf8";

while (<>)
{
    s/(\.\.\.)|(\. \.\.)|(\.\. \.)/\x{2026}/g;
    s/\x27/\x{2019}/g;
    s/([\x{2018}\x{201C}]) /$1/g;
    s/ ([\x{2019}\x{201D}])/$1/g;
    s/:"(?=[A-Za-zçéàèùâêîôûëïüœ])/: \x{00aB}\x{202f}/g;
    s/(?<=[A-Za-zçéàèùâêîôûëïüœ,;:.?!)\x{2026}])"(?=[ ,;:.?!)\x{2026}])/\x{202F}\x{00BB}/g;
    s/(?<=[A-Za-zçéàèùâêîôûëïüœ,;:.?!)\x{2026}])"$/\x{202F}\x{00BB}/g;
    s/ "(?=[A-Za-zçéàèùâêîôûëïüœ(])/ \x{00AB}\x{202F}/g;
    s/^"(?=[A-Za-zçéàèùâêîôûëïüœ(])/\x{00AB}\x{202F}/g;
    s/(?<=[A-Za-z])"(?=[A-Za-z])/\x{2019}/g;
    s/(?<=[A-Za-zçéàèùâêîôûëïüœ,;:.?!)\x{2026}\x{00BB}]) ?(?=[;!?\x{00BB}])/\x{202F}/g;
    s/(?<=[A-Za-zçéàèùâêîôûëïüœ)\x{00BB}]) ?(?=:)/\x{00A0}/g;
    s/(?<=[0-9]) ?(?=%)/\x{00A0}/g;
    s/(?<=\x{00AB}) ?(?=[A-Za-zçéàèùâêîôûëïüœ(])/\x{202F}/g;
    s/([,;:.?!)\x{2026}])([A-Za-zçéàèùâêîôûëïüœ,;:.?!\x{2026}"])/$1 $2/g;
    s/[.\x{2026}?!] [a-zçéàèùâêîôûëïüœ]/\U$&/g;
    s/ ,/,/g;
    s/ \.$/./g;
    s/ \. /. /g;
    s/ \x{2026}/\x{2026}/g;
    s/(\boe)|(oe(?=u|il|sopha|d[èé]m|dipi|str[aou]))/\x{0153}/g;
    s/(?<=\b[Aa]c)oe(?=l[eo])/\x{0153}/g;
    s/(?<=\b[Cc])oe(?=[ln][aeiou])/\x{0153}/g;
    s/(?<=\b[Ff])oe(?=t[aiou])/\x{0153}/g;
    s/(?<=\b[Ff]r)oe(?=b[eé])/\x{0153}/g;
    s/(?<=\b[Hh]om)oe(?=o)/\x{0153}/g;
    s/(?<=\b[Pp])oe(?=cil[aeio])/\x{0153}/g;
    s/(?<=\b[Ss]t)oe(?=chio)/\x{0153}/g;
    s/(?<=str)oe(?=m[ei])/\x{0153}/g;
    s/(?<=\b[Aa]m)oe(?=bicide)/\x{0153}/g;
    s/(?<=\b[Aa]mphic)oe(?=le)/\x{0153}/g;
    s/(?<=\b[Bb]ioc)oe(?=nose)/\x{0153}/g;
    s/(?<=\b[Bb]lastoc)oe(?=le)/\x{0153}/g;
    s/(?<=\b[Bb])oe(?=hmite)/\x{0153}/g;
    s/(?<=\b[Cc])oe(?=rcer)/\x{0153}/g;
    s/(?<=\b[Dd]ianth)oe(?=cia)/\x{0153}/g;
    s/(?<=\b[Dd]icroc)oe(?=liose)/\x{0153}/g;
    s/(?<=\b[Dd]ryoc)oe(?=tes)/\x{0153}/g;
    s/(?<=\b[Ee]ndam)oe(?=bidé)/\x{0153}/g;
    s/(?<=\b[Ee]ntam)oe(?=ba)/\x{0153}/g;
    s/(?<=\b[Ff])oe(?=hn)/\x{0153}/g;
    s/(?<=\b[Ff]ramb)oe(?=sia)/\x{0153}/g;
    s/(?<=\b[Gg]r)oe(?=nendael)/\x{0153}/g;
    s/(?<=\b[Gg]ymnam)oe(?=bien)/\x{0153}/g;
    s/(?<=\b[Ll])oe(?=ss)/\x{0153}/g;
    s/(?<=\b[Mm]elanorh)oe(?=a)/\x{0153}/g;
    s/(?<=\b[Mm])oe(?=ritherium)/\x{0153}/g;
    s/(?<=\b[Mm]on)oe(?=cie)/\x{0153}/g;
    s/(?<=\b[Pp]h)oe(?=nix)/\x{0153}/g;
    s/(?<=\b[Pp]om)oe(?=rium)/\x{0153}/g;
    s/(?<=\b[Rr]h)oe(?=adale)/\x{0153}/g;
    s/(?<=\b[Rr])oe(?=ntgen)/\x{0153}/g;
    s/(?<=\b[Rr])oe(?=sti)/\x{0153}/g;
    s/(?<=\b[Ss]yn)oe(?=cisme)/\x{0153}/g;
    s/(?<=\b[Tt]hécam)oe(?=bien)/\x{0153}/g;
    s/(\bO[Ee](?!M))|(O[Ee](?=[Uu]))/\x{0152}/g;
    print;
}
