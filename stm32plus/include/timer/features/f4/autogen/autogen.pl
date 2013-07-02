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
		my ($portNone,$port1,$port2,$portf,$pinNone,$pin1,$pin2,$pinf,$mode);

		if(length($noremap)>0) { $portNone="GPIO" . substr($noremap,1,1); } else { $portNone="NULL"; }
		if(length($remap1)>0) { $port1="GPIO" . substr($remap1,1,1); } else { $port1="NULL"; }
		if(length($remap2)>0) { $port2="GPIO" . substr($remap2,1,1); } else { $port2="NULL"; }
		if(length($remapf)>0) { $portf="GPIO" . substr($remapf,1,1); } else { $portf="NULL"; }

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
  struct ${feature} {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * \@param remap The remap level (none, partial1, partial2, full)
     */

    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ ${portNone},${port1},${port2},${portf} };
      static const uint16_t pins[4]={ ${pinNone},${pin1},${pin2},${pinf} };
~;

		print {$outfile} qq/
      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER${timerNumber}>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER${timerNumber}>::GPIO_AF);
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
		my ($i,$def1,$def2,$def3,$def4,$fulldef);

		for($i=0;$i<9;$i++) {
				if($i<$featureCount) {
						if($i>0) {
								$def1 .= ",";
								$def2 .= ",";
								$def3 .= ",";
								$def4 .= "\n";
						}

						$def1 .= "class TF${i}=NullTimerGpio";
						$def2 .= "class TF${i}";
						$def3 .= "TF${i}";
						$def4 .= "      TF${i}::initialise(${remapLevel});";
				}
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

  template<TimerGpioRemapLevel TRemapLevel,${def1}>
  struct Timer${timerNumber}GpioFeature;
~;
		}
		
		$fulldef.=qq~

  template<${def2}>
  struct Timer${timerNumber}GpioFeature<${remapLevel},${def3}> : public TimerFeatureBase {
    Timer${timerNumber}GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
${def4}
    }
  };
~;
		print ${outfile} $fulldef;
}
