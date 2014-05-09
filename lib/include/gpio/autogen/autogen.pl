#!/usr/bin/perl -w

use strict;
use warnings;


# generate the enumerations that allow pin details to be encapsulated


my $outfile;


open $outfile, ">", "GpioPinMap.h" or die $!;

&writeHeader;

&writePort("A");
&writePort("B");
&writePort("C");
&writePort("D");
&writePort("E");
&writePort("F");
&writePort("G");
&writePort("H");
&writePort("I");
&writePort("J");
&writePort("K");

&writeTrailer;

close($outfile);

exit 0;


# write out the header

sub writeHeader {

    print $outfile qq[/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 *
 * THIS IS AN AUTOMATICALLY GENERATED FILE - DO NOT EDIT!
 */

#pragma once


namespace stm32plus {
  namespace gpio {];
}


# write out a port and all its pins

sub writePort {

  my ($port)=@_;

  for(my $i=0;$i<16;$i++) {

    print $outfile "\n\n";
    print $outfile "    enum class P${port}${i} {\n";
    print $outfile "      Port=GPIO${port}_BASE,\n";
    print $outfile "      Pin=GPIO_Pin_${i}\n";
    print $outfile "    };" 
  }
}


# write out the trailer

sub writeTrailer {

    print $outfile qq[
  }
}
];

}