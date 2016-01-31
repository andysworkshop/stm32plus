#!/usr/bin/perl -w

use strict;
use warnings;


# Generate the enumerations that allow pin details to be encapsulated
# This script will generate a file in the same directory alled GpioPinMap.h
# which should be moved one directory level up.

my $outfile;


open $outfile, ">", "GpioPinMap.h" or die $!;

&writeHeader;

&writePort("A");
&writePort("B");
&writePort("C");
&writePort("D");
&writePort("E");
&writePort("F");

print $outfile "\n\n#if defined(STM32PLUS_F1) || defined(STM32PLUS_F4)";

&writePort("G");

print $outfile "\n\n#if defined(STM32PLUS_F4)";

&writePort("H");
&writePort("I");
&writePort("J");
&writePort("K");

print $outfile "\n\n#endif  // F4\n#endif  // F1 or F4\n";

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

    print $outfile qq!

    struct P${port}${i} {
      enum {
        Port=GPIO${port}_BASE,
        Pin=GPIO_Pin_${i}
      };
    };! 
  }
}


# write out the trailer

sub writeTrailer {

    print $outfile qq[
  }
}
];

}