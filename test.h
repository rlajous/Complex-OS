#ifndef TEST_H
#define TEST_H

#include "application.h"
#include "assert.h"


void testFileParser();

void testSlaves();

void givenAnAmountOfSlaves();

void whenSlaveISCreated();

void thenSlaveSuccess();

void thenSlaveComunicationIsChecked();

void givenFiles();

void givenArgvFiles();

void whenFilesParsedToString();

void thenCheckAreTheSameGiven();

void thenSlavesAreSuccsesfullyCreated();

void thenCheckIsNotNull();

void thenFilesSuccess();

#endif