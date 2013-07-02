#!/usr/bin/perl -w


use strict;
use warnings;


my ($dma,$stream,$channel,$allinclude,%periphMap);

open $allinclude, ">", "DmaIncludes.h" or die $!;

print {$allinclude} qq!
/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

!;

for($dma=1;$dma<=2;$dma++) {
		for($stream=0;$stream<=7;$stream++) {
				for($channel=0;$channel<=7;$channel++) {
						&writeHeader;
				}
		}
}

close($allinclude);


sub writeHeader {

		my ($infile,$outfile);

		print {$allinclude} "#include \"Dma${dma}Channel${channel}Stream${stream}.h\"\n";

		open $infile, "<", "template" or die $!;
		open $outfile, ">", "Dma${dma}Channel${channel}Stream${stream}.h" or die $!;

		while(<$infile>) {

				if($_ =~ /%PERIPHERALS%/) {
						&writePeripherals($outfile);
				}
				else {

						$_ =~ s/Dma\d/Dma${dma}/;
						$_ =~ s/Channel\d/Channel${channel}/;
						$_ =~ s/Stream\d/Stream${stream}/;
				
						$_ =~ s/DMA\d/DMA${dma}/;
						$_ =~ s/Channel_\d/Channel_${channel}/;
				
						$_ =~ s/TCIF\d/TCIF${stream}/;
						$_ =~ s/HTIF\d/HTIF${stream}/;
						$_ =~ s/TEIF\d/TEIF${stream}/;

						print {$outfile} $_;
				}
		}

		close($infile);
		close($outfile);
}


sub writePeripherals {

		my $outfile=$_[0];
		my(@matches,$periphs,$periph,$line,$csvfile,$i);

		open $csvfile, "<", "dma${dma}.csv" or die $!;

		# seek to the channel

		for($i=0;$i<$channel;$i++) {
				<$csvfile>;
		}

		# get the peripherals for this stream

		$line=<$csvfile>;
		chomp($line);

		@matches=($line =~ m/^(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*)$/);
		$periphs=$matches[$stream];

		if(defined($periphs) && length($periphs)>0) {

				print {$outfile} qq!
  /**
   * Types for the peripherals mapped to this channel
   */
!;

				foreach $periph (split(/\+/,$periphs)) {
						writePeripheral($outfile,$periph);
				}
		}
}



sub writePeripheral {

		my ($outfile,$periph)=@_;

		if(exists($periphMap{$periph})) {
				$periph="${periph}Stream${stream}";
		}
		else {
				$periphMap{$periph}="0";
		}

		print {$outfile} qq!
  template<class... Features> using ${periph}DmaChannel=Dma${dma}Channel${channel}Stream${stream}<Features...>;!;
}
