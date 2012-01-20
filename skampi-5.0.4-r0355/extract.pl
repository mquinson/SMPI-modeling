#!/usr/bin/perl -w

# SKaMPI   MPI-Benchmark
# 
# Copyright 2003-2004 Michael Haller
#           2005 Thomas Worsch  (bug fixes) and
#                major overhaul in order to
#                * make it much more robust concerning whitespace in 
#                  the parsed source code
#                * perform several consistency checks
# Lehrstuhl Informatik fuer Naturwissenschaftler und Ingenieure 
# Fakultaet fuer Informatik
# University of Karlsruhe
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of version 2 of the GNU General Public License as
# published by the Free Software Foundation
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 

################################################################################
################################################################################
# extract.pl		generates an interface file on stdout (typically
#                       piped to "m.c") from measurement source files 
#			for use with the SKaMPI benchmark
#
#			initial version: extract.py by Werner Augustin
#			Perl port by Michael Haller 2004-01-30
#                       modifications by Thomas Worsch 2005 
################################################################################
################################################################################

use strict;

# if one of the consistency checks fails, $error_flag is set to 1 and 
# we will abort without generating anything on stdout
my $error_flag = 0;

# we use $program in messages from this program
my $program = (split '/', $0)[-1];  # last component of path

my $filename;
my $prefix;
my $funcname;

sub print_error
{
    my $msg = shift;
    my $report_func = shift;

    chomp($msg);

    print STDERR "ERROR: $msg \n";
    print STDERR "       file: $filename";
    if ($report_func) {
        print STDERR ",  function: $prefix\_$funcname";
    }
    print STDERR "\n";

    $error_flag = 1;
}

#sub print_warning
#{
#    my $msg = shift;
#    my $report_func = shift;
#
#    chomp($msg);
#
#    print "/* WARNING: $msg \n";
#    print "       file: $filename";
#    if ($report_func) {
#        print ",  function: $prefix\_$funcname";
#    }
#    print "*/\n";
#}

################################################################################
# norm_ws		"normalize" white space in a string
#                       i.e. replace every sequence of whitespace characters by
#                       a single space and remove even those if they are near
#                       a star * (in types) or a comma , (in declaration lists)
#                       or at the beginning or the end of the string.
#
#	inputs:		a string (scalar)
#	outputs:	a string (scalar)
################################################################################
sub norm_ws{
    my $s = shift;
    $s =~ s/\s+/ /g;
    $s =~ s/^\s//g;
    $s =~ s/\s$//g;
    $s =~ s/void\s*\*\s*\*/void\*\*/g;
    $s =~ s/char\s*\*/char\*/g;
    $s =~ s/ ,/,/g;
    $s =~ s/, /,/g;
    return $s;
}

################################################################################
# the array contains for each parseable type (as shitespace normalized string)
#    in [0] the "union member name"
#    in [1] the "return type"
#    in [2] the "descriptor for interfaces string"
#    in [3] the "store function suffix"
my %translations = 
    (
     "void"         => [""         ,"TYPE_VOID"    ,""  ,""        ] ,
     "void**"       => [""         ,""             ,""  ,""        ] ,
     "int"          => ["intv"     ,"TYPE_INTEGER" ,"i" ,"int"     ] ,
     "double"       => ["doublev"  ,"TYPE_DOUBLE"  ,"f" ,"double"  ] ,
     "char*"        => ["stringv"  ,"TYPE_STRING"  ,"s" ,"string"  ] ,
     "MPI_Comm"     => ["commv"    ,"TYPE_COMM"    ,"c" ,"comm"    ] ,
     "MPI_Datatype" => ["datatypev","TYPE_DATATYPE","d" ,"datatype"] ,
     "MPI_Info"     => ["infov"    ,"TYPE_INFO"    ,"h" ,"info"    ] ,
     "MPI_Op"       => ["opv"      ,"TYPE_OP"      ,"o" ,"operator"] ,            
     "IArray"       => ["iarrv"    ,"TYPE_IARRAY"  ,"a" ,"iarray"  ] ,            
     "DArray"       => ["darrv"    ,"TYPE_DARRAY"  ,"e" ,"darray"  ] ,            
     "ERROR"        => ["ERROR"    ,"ERROR"        ,"E" ,"ERROR"   ] ,            
     );

################################################################################
# parse_declaration	simple parser for C parameter declarations
#
#			Supports only some non-composite (primitive) types
#			and some MPI-Handles;  char x[] is treated as char* x.
#                       Anything else cannot be handled by SKaMPI.
#
#	input: 		* a single C parameter declaration string (scalar)
#                         in the form <type> <variable> or in the form   void   .
#                       * a flag; if it is
#                         * 1:  type must be void**
#                         * 0:  type must not be void**
#
#	outputs:	two strings (scalar):
#			type - the C or MPI data type
#			name - the parameter name
#       errors:         if something illegal is passed, the returned
#                       $type is "ERROR" and the $name is the illegal input
################################################################################
sub parse_declaration
{
    $_ = shift;
    my $is_iter = shift;
    my $is_first_arg = shift;
    my ($type, $name);

    if (/^\s* (void) \s*$/x) {
        $type = "void";
        $name = "";
    }
    elsif (/^\s* (void\s*\*\s*\*|int|double|char\s*\*|MPI_Comm|MPI_Datatype|MPI_Info|MPI_Op|IArray|DArray)
          \s* (\w+) \s*$/x){
	$type = norm_ws($1);
	$name=$2;
    }
    elsif (/^\s* (char) \s* (\w+) \s*\[\s*\]\s*$/x){
        $type = "char*";
        $name = $2;
    }
    else {
        print_error("$program could not parse '$_'; illegal parameter type?", 1);
        return ("ERROR", "$_");
    }
    if ( $is_iter ) {
        if ( $is_first_arg && $type ne "void**") {
            print_error("$program didn't find type void** for first argument of iterator.", 1);
            return ("ERROR", "$_");
        }
        if ( ! $is_first_arg && $type eq "void**" ) {
            print_error("$program found type void** for other than the first argument of non-iterator.", 1);
            return ("ERROR", "$_");
        }
    }
    else {
        if ( $type eq "void**" ) {
            print_error("$program found type void** for argument of non-iterator.", 1);
            return ("ERROR", "$_");
        }
    }
    return ($type, $name);
}

################################################################################
# fill_func_ref		bla bla bla
#
#	inputs:		a ref to a hash for a function
#                       a flag indicating whether the ref is for an iterator
#                         which requires some special treatment of the "self"
#
#	outputs:	no return value
#                       modifies the ref'ed hash in place
################################################################################
sub fill_func_ref
{
    my $ref = shift;
    my $is_iter = shift;
    my ($param, $type, $name, $i);
    my @params;
    my @records;

    $ref->{params} = [];
    $ref->{ifacestring} = "";
    @params = split(/,/ ,$ref->{parameterstring} );
    # for iterators we drop first arg
    if ($is_iter) { 
        ($type, $name) = parse_declaration(shift @params, $is_iter, 1);
    } 
    $i = 0;
    foreach $param (@params) {
        ($type, $name) = parse_declaration($param, $is_iter, 0);
        push @{$ref->{params}}, [ $type, $name ];
        $ref->{ifacestring} .= $translations{$type}[2];
        if ($type ne "void") {
            push @records, "par[$i].u.$translations{$type}[0]";
        }
        $i++;
    }
    $ref->{storetype} = $translations{$ref->{returntype}}[3];
    $ref->{parameterstring} = join ", " , @params;
    $ref->{recordstring} = join ", " , @records;
}

################################################################################
# print a help message and exit
################################################################################
sub help
{
    print <<EOM
  Help message for $program. This program understands the following options:

  --help              print this message and exit
  --test=perl      
  --test=parse     
  --test=header
  --test=extern_decls
  --test=interface
  --test=init_code
  --test=all          all of the above test
  --mpicc=<name>      set the name of the compiler to be used (no default)
  --cflags=<flags>    specify flags to be passed to the compiler (default: -E)
  --file=<name>       specify the name of a file to be parsed (no default).
                      This option can be used repeatedly, in which case all the
                      files will be parsed.
  --dir=<name>        specify the name of a directory, from which ALL files with
                      names ending with '.c' will be parsed (default: .)
                      this option is only used if no files are specified via --file

  If --test is given, but no --file or --dir, then no files will be parsed.

  For each of the options --mpicc, --cflags, --dir: 
                   If given repeatedly, the last one wins.
EOM
;

    exit; 
}

################################################################################
# global variables and a few first statements to find out
# whether we are running in production mode or in testing mode
################################################################################
my $dirname = "";       # name of dir from which to get the files
my $dir;		# directory handle
my @files = ();		# all the files (if any) to be parsed

my $do_tests = 0;
my $do_test_perl = 0;
my $do_test_parse = 0;
my $do_test_header = 0;
my $do_test_extern_decls = 0;
my $do_test_interface = 0;
my $do_test_init_code = 0;

my $mpicc_name  = "";         # name of MPI compiler
my $compiler_options = "-E";  

sub parse_args
{
    my $arg;
    foreach $arg (@ARGV) {
        if ($arg =~ /--test=all$/) {
            $do_test_perl=1;
            $do_test_parse=1;
            $do_test_header=1;
            $do_test_extern_decls=1;
            $do_test_interface=1;
            $do_test_init_code=1;
        }
        elsif ($arg =~ /--test=perl$/) {
            $do_test_perl=1;
        }
        elsif ($arg =~ /--test=parse$/) {
            $do_test_parse=1;
        }
        elsif ($arg =~ /--test=header$/) {
            $do_test_header=1;
        }
        elsif ($arg =~ /--test=extern_decls$/) {
            $do_test_extern_decls=1;
        }
        elsif ($arg =~ /--test=interface$/) {
            $do_test_interface=1;
        }
        elsif ($arg =~ /--test=init_code$/) {
            $do_test_init_code=1;
        }
        elsif ($arg =~ /--mpicc=(.*)/) {
            $mpicc_name = $1;
        }
        elsif ($arg =~ /--cflags=(.*)/) {
            $compiler_options = $1;
        }
        elsif ($arg =~ /--dir=(.*)/) {
            $dirname = $1;
        }
        elsif ($arg =~ /--file=(.*)/) {
            push @files, $1;
        }
        elsif ($arg =~ /--help$/) {
            help();
        }
        else {
            print STDERR "unknown arg: $arg\n";
            $error_flag = 1;
        }
    }
    $do_tests = $do_test_perl || $do_test_parse || $do_test_header 
        || $do_test_extern_decls || $do_test_interface || $do_test_init_code;

    if ($error_flag) {
        print STDERR "ERROR: $program called with unknown command line options\nABORTING now.\n";
        exit 1;
    }
}

sub check_args
{
    my $i;
    my $j;
    my @cfiles;
    if (0 == scalar @files) {
        if($dirname eq "" && ! $do_tests) { $dirname = "."; }
        if($dirname ne "") {
            opendir $dir, $dirname;
            @files = readdir $dir;
        }
    }
    
    # there's almost certainly a better way for this, 
    # but I don't know perl well enough
    foreach $filename (@files) {
        if($dirname eq "") { $dirname = "."; }
        if ($filename =~ /\.c$/) { # only parse .c files
            push @cfiles, $filename;
        }
    }
    @files = @cfiles;
}

parse_args();
check_args();
if ($do_tests) {
    run_tests();
}
else {
    run_production_mode();  
}

# in this hash we hold the informations about all functions by name
# it is populated during parse_file and used by all the output_ routines
my %funcs;  

################################################################################
# Do some more checks on the defined functions, which depend on *several* functions
#    Some checks have already been made during parseing.
################################################################################
sub check_funcs
{
    my $f;
    for $f (sort keys(%funcs)) {
        $funcname = $f;
        my $funcref = $funcs{$funcname};
        my $p;
        # ------------------------------------------------------------------
        # first check that all init and finalize functions have the same 
        # interface as the corresponding measure function
        if (exists($funcref->{measure})) {
            if (exists($funcref->{init})) {
                if ( $funcref->{init}{ifacestring} ne $funcref->{measure}{ifacestring} ) {
                    print_error("$program found differing parameter lists for init_$funcname and measure_$funcname", 0);
                }
            }
#            if (exists($funcref->{init})) {
#                if ( $funcref->{init}{parameterstring} ne $funcref->{measure}{parameterstring} ) {
#                    print_warning("$program found differing normalized parameter list strings;\n * WARNING  init_$funcname has '$funcref->{init}{parameterstring}' and \n* WARNING  measure_$funcname has '$funcref->{measure}{parameterstring}'\n", 0);
#                }
#            }
            if (exists($funcref->{finalize})) {
                if ( $funcref->{finalize}{ifacestring} ne $funcref->{measure}{ifacestring} ) {
                    print_error("$program found differing parameter lists for finalize_$funcname and measure_$funcname", 0);
                }
            }
        }
        # ------------------------------------------------------------------
        # make sure that for each init and finalize function there is a corresponding measure function
        if ( exists($funcref->{init}) && ! exists($funcref->{measure}) ) {
            print_error("$program found init_$funcname but no measure_$funcname", 0);
        }
        if ( exists($funcref->{finalize}) && ! exists($funcref->{measure}) ) {
            print_error("$program found finalize_$funcname but no measure_$funcname", 0);
        }
        # ------------------------------------------------------------------
        # make sure that there was no func_ function with the same suffix as a measure_
        if (exists($funcref->{func}) && exists($funcref->{measure}) ) {
            print_error("$program found func_$funcname as well as measure_$funcname", 0);
        }
        # ------------------------------------------------------------------
        # make sure that there was no func_ function with the same suffix as a iterator__
        if (exists($funcref->{func}) && exists($funcref->{iterator}) ) {
            print_error("$program found func_$funcname as well as iterator_$funcname", 0);
        }
        # ------------------------------------------------------------------
        # make sure that there was no measure_ function with the same suffix as a iterator__
        if (exists($funcref->{measure}) && exists($funcref->{iterator}) ) {
            print_error("$program found measure_$funcname as well as iterator_$funcname", 0);
        }
    }
    if ($error_flag) {
        print STDERR "ABORTING: $program found ERRORS and is exiting now.\n";
        exit 1;
    }
}

sub parse_file
{
    $filename = shift;
    my $extract_mpicc_defines = shift;
    my $inside_skampi_extension_block;  
    my ($s, $param);
    my ($xline, $concat_next_line);


    if ($mpicc_name eq "") {
        die "ERROR: $program doesn't know which compiler to use. "
            . "Please specify using the --mpicc=<compiler> option.\n";
    }

    $inside_skampi_extension_block = 0;
    $concat_next_line = 0;
    open(FHANDLE,"$mpicc_name $extract_mpicc_defines $compiler_options $dirname/$filename |"); 
    while(<FHANDLE>){
        if ($concat_next_line) {chomp($xline);  $xline = $xline . " " . $_;}
        else { $xline = $_; }
        
        if( ! $inside_skampi_extension_block ) {
            next if $xline !~ /\#pragma\s+weak\s+begin_skampi_extensions/;
            $inside_skampi_extension_block = 1; 
        }
        if( $xline =~ /\#pragma\s+weak\s+end_skampi_extensions/ ) {
            $inside_skampi_extension_block = 0;
            next;
        }
        
        my $re_returntype = "void|int|double|char\\s*\\*|MPI_Comm|MPI_Datatype|MPI_Info|MPI_Op|IArray|DArray";
        my $re_prefix = "measure|init|finalize|func|iterator";
        if ($xline =~ / \s* ($re_returntype) \s* ($re_prefix)_(\w+) /x) {
            # found beginning of function declaration
            if ($xline =~ / \s* ($re_returntype) \s* ($re_prefix)_(\w+) \s* \((.*)\) /x) {
                # declaration is complete 
                my $returntype = $1;
                $prefix = $2;
                $funcname = $3;
                my $parameterstring = $4;
                if ($funcname =~ /^[0123456789]\w*$/) {
                    print_error("$program found function name suffix starting with a digit.", 1);
                }
                my $ref = $funcs{$funcname}{$prefix};
                $ref->{returntype} = norm_ws($returntype);
                $ref->{parameterstring} = norm_ws($parameterstring);
                if ($prefix eq "iterator" && $returntype eq "void") {
                    print_error("$program found an iterator with return type void.\n", 1);
                }
                if ($prefix eq "measure" && $returntype ne "double") {
                    print_error("$program found a measure_ function not returning a double.\n", 1);
                }
                if ($prefix eq "init" && $returntype ne "void") {
                    print_error("$program found an init_ function with non-void return type.\n", 1);
                }
                if ($prefix eq "finalize" && $returntype ne "void") {
                    print_error("$program found a finalize_ function with non-void return type.\n", 1);
                }
                fill_func_ref($ref,($prefix eq "iterator"));
                $funcs{$funcname}{$prefix} = $ref;
                $concat_next_line = 0;
            }
            else {
                # we only found the beginning; append subsequent lines until we have also (......)
                $concat_next_line = 1;
                next;
            }
        }
    }

    ##########################################################################
    # now that we have populated the %funcs hash we will do some checks
    # in case of errors, this will die.
    check_funcs();
}

sub output_file_header
{
    print <<EOT
/* This file was generated with $program from the source files
 *    @files
 * in directory 
 *    $dirname
 * Do not change this file manually. All changes will be lost, 
 *    next time $program is run to generate this file.
 */
        
#include <stdlib.h>
#include <mpi.h>

#include "mpiversiontest.h"
        
#include "misc.h"
#include "exec.h"
#include "symboltable.h"
        
EOT
        ;
}

sub output_extern_func_decls
{
    my $funcname;
    foreach $funcname (sort keys(%funcs) ){
        my $funcref = $funcs{$funcname};
        
        if( exists($funcref->{measure}) ){
            print "extern double measure_$funcname( $funcref->{measure}{parameterstring} );\n";
        }
        if( exists($funcref->{init}) ){
            print "extern void init_$funcname( $funcref->{init}{parameterstring} );\n";
        }
        if( exists($funcref->{finalize}) ){
            print "extern void finalize_$funcname($funcref->{finalize}{parameterstring} );\n";
        }
        if( exists($funcref->{func}) ){
            print "extern ".$funcref->{func}{returntype}." func_$funcname( $funcref->{func}{parameterstring} );\n";
        }
        if( exists($funcref->{iterator}) ){
            print "extern ".$funcref->{iterator}{returntype}." iterator_".$funcname."( void** self";
            if ($funcref->{iterator}{parameterstring} ne "") {
                print ", $funcref->{iterator}{parameterstring}";
            }
            print " );\n";
        }
    }
    print "\n";
}

sub output_interface_code
{
    my $funcname;
    foreach $funcname (sort keys(%funcs) ){
        my $funcref = $funcs{$funcname};
        
        # same as above: first treat "measure"s, then "init"s etc.
        if( exists($funcref->{measure}) ){
            print "static void call_measure_$funcname( struct variable *result, "
                . "struct variable par[] )\n";
            print "{ store_double( result, measure_$funcname( ";
            print "$funcref->{measure}{recordstring}";
            print " )); }\n\n";
        }
        
        if( exists($funcref->{init}) ){
            print "static void call_init_$funcname( struct variable *result, "
                . "struct variable par[] )\n";
            print "{ init_$funcname( ";
            print "$funcref->{init}{recordstring}";
            print " ); }\n\n";
        }
        
        if( exists($funcref->{finalize}) ){
            print "static void call_finalize_$funcname( struct variable *result, "
                . "struct variable par[] )\n";
            print "{ finalize_$funcname( ";
            print "$funcref->{finalize}{recordstring}";
            print " ); }\n\n";
        }
        
        if( exists($funcref->{func}) ){
            print "static void call_func_$funcname( struct variable *result, "
                . "struct variable par[] )\n";
            
            # functions may have various return types, including "void"
            if($funcref->{func}{returntype} eq "void"){
                print "{ func_$funcname( ";
            }
            # non-void return values have to be stored
            else{
                print "{ store_$funcref->{func}{storetype}( result, func_$funcname( ";
            }
            print "$funcref->{func}{recordstring}";
            # take care about number of parentheses
            if($funcref->{func}{returntype} eq "void"){
                print " ); }\n\n";
            }
            else{
                print " )); }\n\n";
            }
        }
        
        if( exists($funcref->{iterator}) ){
            print "static void call_iterator_$funcname( struct variable *result, "
                ."struct variable par[], void **self )\n";
            
            # iterators have a non-void return type
            print "{ store_$funcref->{iterator}{storetype}( result, iterator_$funcname( self";
            if ("$funcref->{iterator}{recordstring}" ne "") {
                print ", $funcref->{iterator}{recordstring}";
            }
            print " )); }\n\n";
        }
    }
}

sub output_initialization_code
{
    my $funcname;

    print "void init_functions(void)\n{\n";
    
    foreach $funcname (sort keys(%funcs) ){
        my $funcref = $funcs{$funcname};
        my $functyperef;
        
        if( exists($funcref->{iterator}) ){
            print "  insert_iterator( &call_iterator_$funcname, ";
        }
        else{
            print "  insert_function( ";
            
            print "&call_init_$funcname, " if( exists($funcref->{init}) );
            print "NULL, " unless( exists($funcref->{init}) );
            
            print "&call_measure_$funcname, " if( exists($funcref->{measure}) ); 
            print "&call_func_$funcname, " unless( exists($funcref->{measure}) );
            
            print "&call_finalize_$funcname, " if( exists($funcref->{finalize}) ); 
            print "NULL, " unless( exists($funcref->{finalize}) );
        }
        
        if (exists($funcref->{measure})) {
            $functyperef = $funcref->{measure};
        }
        elsif (exists($funcref->{iterator})) {
            $functyperef = $funcref->{iterator};
        }
        else {
            $functyperef = $funcref->{func};
        }
        print "\"$funcname\", \"";
        print $functyperef->{ifacestring};
        print "\", ".$translations{$functyperef->{returntype}}[1]." );\n";
    }
    print "}\n";

}
    
sub output_ifdef
{
    my $onesided = shift;
    my $mpi_io = shift;
      
    print "\n#if ";
    unless ($onesided) { print "!"; }
    print "defined(USE_ONESIDED) && ";
    unless ($mpi_io) { print "!"; }
    print "defined(USE_MPI_IO)\n\n";
}

sub output_endif()
{
    print "\n#endif\n\n";
}

sub run_production_mode
{
    my $filename;
    my $extract_mpicc_defines;
    my $use_onesided;
    my $use_mpi_io;
    my @dflag = ("-U", "-D");
    
    output_file_header();

    for ($use_onesided = 0; $use_onesided <= 1; $use_onesided++) {
        for ($use_mpi_io = 0; $use_mpi_io <= 1; $use_mpi_io++) {
            output_ifdef($use_onesided, $use_mpi_io);
            $extract_mpicc_defines = "-D __SKAMPI_EXTRACT_IS_RUNNING__ " . 
                $dflag[$use_onesided] . "USE_ONESIDED " .
                $dflag[$use_mpi_io] . "USE_MPI_IO";
        
            %funcs = ();
            foreach $filename (@files){
                parse_file($filename, $extract_mpicc_defines);
            }
            
            output_extern_func_decls();
            output_interface_code();
            output_initialization_code();
            output_endif();
        }
    }
}

sub run_test_perl
{
    my ($s, $type, $name);
    
    print "######## running perl tests ###############################################\n";
    print "**** some tests of norm_ws ************************************************\n";
    foreach $s ("tab	tab	x", "    many      spaces        ",
                "\nnewline\nnewline", " int i , int j ", " \t\n", "") {
        print "string='$s'\n==> normalized to: '";
        print norm_ws($s);
        print "'\n";
    }

    print "**** some tests of parse_declaration **************************************\n";
    foreach $s ("void", "void** ptr", " void * * ptr", "int i", "double x",
               "char* s", " char * s", " char *s", "char s[]", "char s   [ ]", 
               "MPI_Comm comm", "MPI_Datatype dt", "MPI_Op op", "MPI_Info h",
              # "junk", "int* y", "void * p"
                ) {
        print "param declaration='$s'\n";
        ($type, $name) = parse_declaration($s);
        print "\$type='$type', \$name='$name'\n\n";
    }
}

sub run_test_parse
{
    my $funcname;
    
    # we assume that %funcs has already been poplulated (by call(s) to parse_file)
    print "######## running parse tests ##############################################\n";
    foreach $funcname ( sort keys(%funcs) ){
        print "....$funcname:\n";
        my $funcref = $funcs{$funcname};
        my $functype;
        
        foreach $functype (sort keys(%{$funcref})) {
            print "........$functype:\n";
            my $functyperef = $funcref->{$functype};
            my $property;
            foreach $property (sort keys( %{ $functyperef } )){
                print "............$property = $functyperef->{$property}\n";
            }
        }
    }            
}

sub run_test_header
{
    print "######## running header test ##############################################\n";
    output_file_header();
}

sub run_test_extern_decls
{
    print "######## running extern decls test ########################################\n";
    output_extern_func_decls();
}

sub run_test_init_code
{
    print "######## running initialization code test #################################\n";
    output_initialization_code();
}

sub run_test_interface
{
    print "######## running interface code test ######################################\n";
    output_interface_code();
}

sub run_tests
{
    my $s;
    my ($type, $name);
    my $filename;
    my $extract_mpicc_defines;
    my $use_onesided;
    my $use_mpi_io;
    my @dflag = ("-U", "-D");

    print "###########################################################################\n";
    print "######## $program: running in test mode now ###############################\n";
    print "###########################################################################\n";

    print "#### first some data collected from command line args #####################\n";
    print "##  \$mpicc_name = '$mpicc_name'\n";
    print "##  \$compiler_options = '$compiler_options'\n";
    print "##  \@files = '@files'\n";
    print "##  \$dirname = '$dirname'\n";
    print "##  tests requested:";
    if ($do_test_perl) { print " perl "; }
    if ($do_test_parse) { print " parse "; }
    if ($do_test_header) { print " header "; }
    if ($do_test_extern_decls) { print " extern_decls "; }
    if ($do_test_interface) { print " interface "; }
    if ($do_test_init_code) { print " init_code "; }
    print "\n";

    if ($do_test_perl) { run_test_perl(); }
    if ($do_test_header) { run_test_header(); }

    for ($use_onesided = 0; $use_onesided <= 1; $use_onesided++) {
        for ($use_mpi_io = 0; $use_mpi_io <= 1; $use_mpi_io++) {
            if ($do_test_extern_decls || $do_test_interface ||$do_test_init_code ) {
                output_ifdef($use_onesided, $use_mpi_io);
            }
            $extract_mpicc_defines = "-D __SKAMPI_EXTRACT_IS_RUNNING__ " . 
                $dflag[$use_onesided] . "USE_ONESIDED " .
                $dflag[$use_mpi_io] . "USE_MPI_IO";
            %funcs = ();

            foreach $filename (@files){
                parse_file($filename, $extract_mpicc_defines);
            }
            if ($do_test_parse) { run_test_parse(); }
            if ($do_test_extern_decls) { run_test_extern_decls(); }
            if ($do_test_interface) { run_test_interface(); }
            if ($do_test_init_code) { run_test_init_code(); }
    
            if ($do_test_extern_decls || $do_test_interface ||$do_test_init_code ) {
                output_endif();
            }
        }
    }
    
    print "######## this is the end of the tests #####################################\n";
}
