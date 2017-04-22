#!/usr/bin/perl -w


use strict;
use warnings;


&genGpio;
print "Done\n";
exit 0;


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
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 *
 * THIS IS AN AUTOMATICALLY GENERATED FILE - DO NOT EDIT!
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif

namespace stm32plus {
];
}

#
# write a feature
#

sub writeFeature {

		my ($outfile,$feature,$noremap,$remap1,$remap2,$remapf,$timerNumber)=@_;
		my ($portNone,$port1,$port2,$portf,$pinNone,$pin1,$pin2,$pinf,$mode);

		if(length($noremap)>0) { $portNone="GPIO" . substr($noremap,1,1) . "_BASE"; } else { $portNone="0"; }
		if(length($remap1)>0) { $port1="GPIO" . substr($remap1,1,1) . "_BASE"; } else { $port1="0"; }
		if(length($remap2)>0) { $port2="GPIO" . substr($remap2,1,1) . "_BASE"; } else { $port2="0"; }
		if(length($remapf)>0) { $portf="GPIO" . substr($remapf,1,1) . "_BASE"; } else { $portf="0"; }

		if(length($noremap)>0) { $pinNone="GPIO_Pin_" . substr($noremap,2); } else { $pinNone="0"; }
		if(length($remap1)>0) { $pin1="GPIO_Pin_" . substr($remap1,2); } else { $pin1="0"; }
		if(length($remap2)>0) { $pin2="GPIO_Pin_" . substr($remap2,2); } else { $pin2="0"; }
		if(length($remapf)>0) { $pinf="GPIO_Pin_" . substr($remapf,2); } else { $pinf="0"; }

		writeFeatureStruct($outfile,$feature,$portNone,$port1,$port2,$portf,$pinNone,$pin1,$pin2,$pinf,"GPIO_Mode_IN_FLOATING",$timerNumber) if($feature =~ m/ETR/);
		writeFeatureStruct($outfile,$feature,$portNone,$port1,$port2,$portf,$pinNone,$pin1,$pin2,$pinf,"GPIO_Mode_IN_FLOATING",$timerNumber) if($feature =~ m/BKIN/);
		writeFeatureStruct($outfile,$feature,$portNone,$port1,$port2,$portf,$pinNone,$pin1,$pin2,$pinf,"GPIO_Mode_AF_PP",$timerNumber) if($feature =~ m/CH\dN/);

		if($feature =~ m/CH\d$/) {
				writeFeatureStruct($outfile,"${feature}_IN",$portNone,$port1,$port2,$portf,$pinNone,$pin1,$pin2,$pinf,"GPIO_Mode_IN_FLOATING",$timerNumber);
				writeFeatureStruct($outfile,"${feature}_OUT",$portNone,$port1,$port2,$portf,$pinNone,$pin1,$pin2,$pinf,"GPIO_Mode_AF_PP",$timerNumber);
		}
}

#
# write a feature structure
#
			
sub writeFeatureStruct {
	
		my ($outfile,$feature,$portNone,$port1,$port2,$portf,$pinNone,$pin1,$pin2,$pinf,$mode,$timerNumber)=@_;
 
		print {$outfile} qq~
  template<TimerGpioRemapLevel TRemapLevel>
  struct ${feature} {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * \@tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    ${feature}() {

      static constexpr const uint32_t ports[4]={ ${portNone},${port1},${port2},${portf} };
      static constexpr const uint16_t pins[4]={ ${pinNone},${pin1},${pin2},${pinf} };
~;

		if($mode eq "GPIO_Mode_IN_FLOATING") {
				print {$outfile} qq/
      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER${timerNumber}>::GPIO_SPEED,Gpio::PUPD_NONE); /;
		}
		elsif($mode eq "GPIO_Mode_AF_PP") {
				print {$outfile} qq/
      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER${timerNumber}>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER${timerNumber},ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF); /;
		}

		print {$outfile} qq/
    }
  };
/;

}


#
# write the closing trailer
#

sub writeClosing {

		my ($outfile,$timerNumber,$featureCount,$hasNone,$hasPartial1,$hasPartial2,$hasFull)=@_;

		writeClosingDef($outfile,$timerNumber,$featureCount,"TIMER_REMAP_NONE",undef) if($hasNone==1);

		if($hasPartial1==1 && $hasPartial2==1) {
				writeClosingDef($outfile,$timerNumber,$featureCount,"TIMER_REMAP_PARTIAL1","GPIO_PartialRemap1_TIM${timerNumber}");
				writeClosingDef($outfile,$timerNumber,$featureCount,"TIMER_REMAP_PARTIAL2","GPIO_PartialRemap2_TIM${timerNumber}");
		}
		elsif($hasPartial1==1) {
				writeClosingDef($outfile,$timerNumber,$featureCount,"TIMER_REMAP_PARTIAL1","GPIO_PartialRemap_TIM${timerNumber}");
		}
		elsif($hasPartial2==1) {
				writeClosingDef($outfile,$timerNumber,$featureCount,"TIMER_REMAP_PARTIAL2","GPIO_PartialRemap_TIM${timerNumber}");
		}

		if($hasFull) {

				if($hasPartial1 || $hasPartial2) {
						writeClosingDef($outfile,$timerNumber,$featureCount,"TIMER_REMAP_FULL","GPIO_FullRemap_TIM${timerNumber}") if($hasFull==1);
				}
				else {
						writeClosingDef($outfile,$timerNumber,$featureCount,"TIMER_REMAP_FULL","GPIO_Remap_TIM${timerNumber}") if($hasFull==1);
				}
		}

		print {$outfile} "}\n";

		close($outfile);
}


sub writeClosingDef {

		my ($outfile,$timerNumber,$featureCount,$remapLevel,$gpioRemap)=@_;
		my ($def,$fulldef);

		$def="      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);";

		if(defined($gpioRemap)) {
				$def .= "\n      GPIO_PinRemapConfig(${gpioRemap},ENABLE);";
		}

		# generic definition if this is the first call

		if($remapLevel eq "TIMER_REMAP_NONE") {

				$fulldef=qq~
  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer${timerNumber}GpioFeature<REMAP_NONE,TIM${timerNumber}_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,template<TimerGpioRemapLevel> class... Features>
  struct Timer${timerNumber}GpioFeature;
~;
		}
		
		$fulldef.=qq~

  template<template<TimerGpioRemapLevel> class... Features>
  struct Timer${timerNumber}GpioFeature<${remapLevel},Features...> : TimerFeatureBase, Features<${remapLevel}>... {
    Timer${timerNumber}GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
${def}
    }
  };
~;
		print ${outfile} $fulldef;
}
