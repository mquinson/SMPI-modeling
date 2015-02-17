#!/usr/bin/perl -w

# This script takes a Pajé trace and produce a datafile that can be loaded into R to allow statistical analysis
# An example of use is given in this sweave document:
#     http://mescal.imag.fr/membres/arnaud.legrand/blog/2012/01-january/SMPI_pt2pt_analysis.pdf

use strict;
use warnings;

open INPUT, $ARGV[0] or die;
my($line);
my(@Statelist);
my($State);

while(defined($line=<INPUT>)) {
    if($line=~/^6 /) {
	my($state,$date,$rank,$junk,$type,$id) = split(/\s+/,$line);
	$rank =~ s/\D*//g;
#	print "[$date $rank] $type\n";
	$$State[$rank] = [$type,$date];
    }
    if($line=~/^7 /) {
	my($state,$date,$rank) = split(/\s+/,$line);
	$rank =~ s/\D*//g;
#	print "[$date $rank] close state\n";
	push @{$$State[$rank]}, $date;
	push @{$Statelist[$rank]}, $$State[$rank];
    }
    if($line=~/^8 /) {
	my($state,$date,$container_id,$container_type,$rank,$type,$key,$size,$id) = split(/\s+/,$line);
	$rank =~ s/\D*//g;
#	print "[$date $rank] $size\n";
	unshift @{$$State[$rank]}, $size;
    }
}

foreach my $rank (0..$#Statelist) {
    foreach my $event (@{$Statelist[$rank]}) {
    }
}


(scalar(@{$Statelist[0]}) == scalar(@{$Statelist[1]})) or die "Incompatible number of events";
my $n = @{$Statelist[0]};
for(my $e=0;$e<$n;$e++) {
    if((scalar(@{$Statelist[0][$e]})==3) &&
       (scalar(@{$Statelist[1][$e]})==3)) {
	print "0 "
    }
    print "@{$Statelist[0][$e]} ";
    print "@{$Statelist[1][$e]}\n";
}
