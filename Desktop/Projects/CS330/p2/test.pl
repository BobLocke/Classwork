#!/usr/bin/perl

use strict;
use Math::Complex;

die "usage: $0 <poly.in> <roots.out>\n" unless @ARGV == 2;

my @poly;
open(POLY,$ARGV[0]) or die "$!\n";
for (<POLY>) {
    /([-+]?\d+(\.\d*)?)\s+([-+]?\d+(\.\d*)?)/;
    my $z = $1 + $3*i;
    push @poly, $z;
}
close POLY;

my @roots;
open(ROOTS,$ARGV[1]) or die "$!\n";
for (<ROOTS>) {
    if (/z\[(\d+)\]\s*=\s*([-+]?\d+(\.\d*)?)\s*\+\s*([-+]?\d+(\.\d*)?)\s*i/) {
	my $j = $1;
	my $a = $2 + $4*i;
	$roots[$j] = $a;
    }
}
close ROOTS;

die "bogus number of roots!\n" unless  @roots == @poly;

sub evalPoly {
    my $z = shift;
    my $n = @_;
    my $f = $z + $_[$n-1];
    for (my $k = $n-2; $k >= 0; $k--) {
	$f = $f*$z + $_[$k];
    }
    return $f;
}

my $epsilon = 1e-5;

foreach my $z (@roots) {
    my $p = evalPoly($z, @poly);
    if (abs($p) <= $epsilon) {
	print "GOOD: ";
    } else {
	print "BAD!: "
    }
    print "f($z) = $p\n";
}

exit 0;
