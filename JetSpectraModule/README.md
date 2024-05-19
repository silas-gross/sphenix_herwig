# hepmc_jet_module
A module to evaluate the spectra produced by a MC model that outputs to HepMC2 files using the sPHENIX Fun4All framework

## General philosophy 

This jet module takes in some data in a hepmc format, process it and outputs a set of kinematics on the partons coming out of the intial vertex, and the final state particles for each event independently. 

## Subroutines 
###JetID
The JetID routine allows for tracking along all the daughter particles from each originating parton and collecting all final state particles into a vector that then has its own kinematics calculated. 
This gives a "truth" value of all kinematics on the individual jets, independent of selection criterion.

###Pythia 
This generates interal pythia events with the relevant jet trigger inside of the subroutines to act as a cross check

###JetFinding 
Using fastjet a number of different jet identification methods are integrated to allow for comparision of methods and the values for the kinematics measured in the originating jet

###Jet object
This creates a basic jet object that will be handed off to other analysis tools that interact with this module without having to build the full fun4all jet object and allow for low level checks of observables as a first order
