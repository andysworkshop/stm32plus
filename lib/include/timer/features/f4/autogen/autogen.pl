#!/usr/bin/perl -w


use strict;
use warnings;


&genGpio;
print "Done\n";
exit 0;

my ($pinsNoneText,$pinsRemap1Text,$pinsRemap2Text,$pinsRemapFullText,$featuresText);

#
# generate GPIO headers
#

sub genGpio {

		my ($infile,$outfile,$str,$feature,$noremap,$remap1,$remap2,$remapf,$filename,$timerNumber,$featureCount);
		my ($hasNone,$hasPartial1,$hasPartial2,$hasFull,$line);

		open $infile, "<", "gpio-mapping.csv" or die $!;

		# skip header

		<$infile>;

		# read lines

		$hasNone=$hasPartial1=$hasPartial2=$hasFull=0;
		$timerNumber="";

		LINE: while(<$infile>) {

				# trim the line

				$line=$_;
				$line =~ s/\r|\n//g;
				next LINE if(length($line)==0 || $line eq ",,,,");

				# check if we need to move on

				$line =~ m/TIM(\d+)/;
				if($1 ne $timerNumber) {

						writeClosing($outfile,$timerNumber,$featureCount,$hasNone,$hasPartial1,$hasPartial2,$hasFull)
								if(defined($outfile));

            $pinsNoneText=$pinsRemap1Text=$pinsRemap2Text=$pinsRemapFullText=$featuresText="";

						$timerNumber=$1;
						$filename="Timer${timerNumber}GpioFeature.h";
						open $outfile, ">", $filename or die $!;

						writeHeader($outfile);
						$featureCount=0;

						$hasNone=$hasPartial1=$hasPartial2=$hasFull=0;	
				}
		
				$line =~ m/^(.*),(.*),(.*),(.*),(.*)$/;

				($feature,$noremap,$remap1,$remap2,$remapf)=($1,$2,$3,$4,$5);

				$hasNone=1 if(length($noremap)>0);
				$hasPartial1=1 if(length($remap1)>0);
				$hasPartial2=1 if(length($remap2)>0);
				$hasFull=1 if(length($remapf)>0);

				writeFeature($outfile,$feature,$noremap,$remap1,$remap2,$remapf,$timerNumber);
				$featureCount++;
		}
		writeClosing($outfile,$timerNumber,$featureCount,$hasNone,$hasPartial1,$hasPartial2,$hasFull);
}

#
# write file header
#

sub writeHeader {

		print {$_[0]} qq[/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 to 2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 *
 * THIS IS AN AUTOMATICALLY GENERATED FILE - DO NOT EDIT!
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif

namespace stm32plus {
];
}

#
# write a feature
#

sub writeFeature {

		my ($outfile,$feature,$noremap,$remap1,$remap2,$remapf,$timerNumber)=@_;

    $pinsNoneText.="\n    typedef gpio::${noremap} ${feature}_Pin;" if(length($noremap)>0);
    $pinsRemap1Text.="\n    typedef gpio::${remap1} ${feature}_Pin;" if(length($remap1)>0);
    $pinsRemap2Text.="\n    typedef gpio::${remap2} ${feature}_Pin;" if(length($remap2)>0);
    $pinsRemapFullText.="\n    typedef gpio::${remapf} ${feature}_Pin;" if(length($remapf)>0);

		writeFeatureStruct($outfile,$feature,$feature,$timerNumber) if($feature =~ m/ETR/);
		writeFeatureStruct($outfile,$feature,$feature,$timerNumber) if($feature =~ m/BKIN/);
		writeFeatureStruct($outfile,$feature,$feature,$timerNumber) if($feature =~ m/CH\dN/);

		if($feature =~ m/CH\d$/) {
				writeFeatureStruct($outfile,"${feature}_IN",$feature,$timerNumber);
        $featuresText.="\n  template<typename TPinPackage> using ${feature}_OUT=${feature}_IN<TPinPackage>;";
		}
}

#
# write a feature structure
#
			
sub writeFeatureStruct {
	
		my ($outfile,$feature,$shortFeature,$timerNumber)=@_;
 
		$featuresText.=qq~

  /**
   * Initialise GPIO pins for this timer GPIO mode
   * \@tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct ${feature} {

    ${feature}() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::${shortFeature}_Pin::Port),
          TPinPackage::${shortFeature}_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER${timerNumber}>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER${timerNumber},
          TPinPackage::${shortFeature}_Pin::Port,
          TPinPackage::${shortFeature}_Pin::Pin>::GPIO_AF);
      }
  };
~
}


#
# write the closing trailer
#

sub writeClosing {

	my ($outfile,$timerNumber,$featureCount,$hasNone,$hasPartial1,$hasPartial2,$hasFull)=@_;

  # write the pin packages

  if($hasNone==1) {
    print ${outfile} qq~
  struct TIM${timerNumber}_PinPackage_Remap_None {${pinsNoneText}
  };
~
  }

  if($hasPartial1==1) {
    print ${outfile} qq~
  struct TIM${timerNumber}_PinPackage_Remap_Partial1 {${pinsRemap1Text}
  };
~
  }

  if($hasPartial2==1) {
    print ${outfile} qq~
  struct TIM${timerNumber}_PinPackage_Remap_Partial2 {${pinsRemap2Text}
  };
~
  }

  if($hasFull==1) {
    print ${outfile} qq~
  struct TIM${timerNumber}_PinPackage_Remap_Full {${pinsRemapFullText}
  };
~
  }

  # write the features

  print ${outfile} ${featuresText};

  # write the definitions

	writeClosingDef($outfile,$timerNumber,$featureCount,"TIMER_REMAP_NONE","None") if($hasNone==1);
	writeClosingDef($outfile,$timerNumber,$featureCount,"TIMER_REMAP_PARTIAL1","Partial1") if($hasPartial1==1);
  writeClosingDef($outfile,$timerNumber,$featureCount,"TIMER_REMAP_PARTIAL2","Partial2") if($hasPartial2==1);
  writeClosingDef($outfile,$timerNumber,$featureCount,"TIMER_REMAP_FULL","Full") if($hasFull==1);

  print ${outfile} qq~
  /**
   * Custom structure to allow any pin mapping.
   *
   * e.g:
   *    Timer14CustomGpioFeature<TIM14_CH1_OUT<Myclass>>
   * and in "MyClass" you would do a public declaration:
   *    typedef gpio::PF9 TIM14_CH1_Pin;
   */

  template<class... Features>
  struct Timer${timerNumber}CustomGpioFeature : TimerFeatureBase,Features... {
    Timer${timerNumber}CustomGpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };
}
~;

  close($outfile);
}


sub writeClosingDef {

	my ($outfile,$timerNumber,$featureCount,$remapLevel,$remapLevelShort)=@_;

	# generic definition if this is the first call

  if($remapLevel eq "TIMER_REMAP_NONE") {

	  print ${outfile} qq~

  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer${timerNumber}GpioFeature<TIMER_REMAP_NONE,TIM${timerNumber}_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,template<typename> class... Features>
  struct Timer${timerNumber}GpioFeature;
~;
		}
		
		print ${outfile} qq~
  template<template<typename> class... Features>
  struct Timer${timerNumber}GpioFeature<${remapLevel},Features...> : TimerFeatureBase,Features<TIM${timerNumber}_PinPackage_Remap_${remapLevelShort}>... {
    Timer${timerNumber}GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };
~;
}
