/*
********************************************************************************
*   Copyright (C) 2015, International Business Machines
*   Corporation and others.  All Rights Reserved.
********************************************************************************
*
* File DECIMFMT2.H
********************************************************************************
*/

#ifndef DECIMFMT2_H
#define DECIMFMT2_H

#include "unicode/utypes.h"
#include "unicode/uobject.h"
#include "affixpatternparser.h"
#include "sciformatter.h"
#include "digitformatter.h"
#include "digitgrouping.h"
#include "precision.h"
#include "digitaffixesandpadding.h"

U_NAMESPACE_BEGIN

class UnicodeString;
class FieldPosition;
class ValueFormatter;
class FieldPositionHandler;

class DecimalFormat2 : public UMemory {
public:

DecimalFormat2(
        const UnicodeString &pattern,
        DecimalFormatSymbols *symbolsToAdopt,
        UParseError &parseError,
        UErrorCode &status);
DecimalFormat2(const DecimalFormat2 &other);
DecimalFormat2 &operator=(const DecimalFormat2 &other);
~DecimalFormat2();
UnicodeString &format(
        int32_t number,
        UnicodeString &appendTo,
        FieldPosition &pos,
        UErrorCode &status) const;
UnicodeString &format(
        int32_t number,
        UnicodeString &appendTo,
        FieldPositionIterator *posIter,
        UErrorCode &status) const;
UnicodeString &format(
        int64_t number,
        UnicodeString &appendTo,
        FieldPosition &pos,
        UErrorCode &status) const;
UnicodeString &format(
        double number,
        UnicodeString &appendTo,
        FieldPosition &pos,
        UErrorCode &status) const;
UnicodeString &format(
        const DigitList &number,
        UnicodeString &appendTo,
        FieldPosition &pos,
        UErrorCode &status) const;
UnicodeString &format(
        int64_t number,
        UnicodeString &appendTo,
        FieldPositionIterator *posIter,
        UErrorCode &status) const;
UnicodeString &format(
        double number,
        UnicodeString &appendTo,
        FieldPositionIterator *posIter,
        UErrorCode &status) const;
UnicodeString &format(
        const DigitList &number,
        UnicodeString &appendTo,
        FieldPositionIterator *posIter,
        UErrorCode &status) const;

void setMinimumIntegerDigits(int32_t newValue);
void setMaximumIntegerDigits(int32_t newValue);
void setMinimumFractionDigits(int32_t newValue);
void setMaximumFractionDigits(int32_t newValue);
void setScientificNotation(UBool newValue);
void setSignificantDigitsUsed(UBool newValue);
int32_t getMinimumIntegerDigits() const { 
        return fMinIntDigits; }
int32_t getMaximumIntegerDigits() const { 
        return fMaxIntDigits; }
int32_t getMinimumFractionDigits() const { 
        return fMinFracDigits; }
int32_t getMaximumFractionDigits() const { 
        return fMaxFracDigits; }
UBool isScientificNotation() const { return fUseScientific; }
UBool areSignificantDigitsUsed() const { return fUseSigDigits; }
void setGroupingSize(int32_t newValue);
void setSecondaryGroupingSize(int32_t newValue);
void setMinimumGroupingDigits(int32_t newValue);
void setGroupingUsed(UBool newValue);
int32_t getGroupingSize() const { return fGrouping.fGrouping; }
int32_t getSecondaryGroupingSize() const { return fGrouping.fGrouping2; }
int32_t getMinimumGroupingDigits() const { return fGrouping.fMinGrouping; }
UBool isGroupingUsed() const { return fUseGrouping; }
void setCurrency(const UChar *currency, UErrorCode &status);
void applyPattern(const UnicodeString &pattern, UErrorCode &status);

private:
// These fields include what the user can see and set.
// When the user updates these fields, it triggers automatic updates of
// other fields that may be invisible to user

// Updating any of the following fields triggers an update to
// fEffPrecision.fMantissa.fMin,
// fEffPrecision.fMantissa.fMax,
// fEffPrecision.fMantissa.fSignificant fields
// We have this two phase update because of backward compatibility. 
// DecimalFormat has to remember all settings even if those settings are
// invalid or disabled.
int32_t fMinIntDigits;
int32_t fMaxIntDigits;
int32_t fMinFracDigits;
int32_t fMaxFracDigits;
int32_t fMinSigDigits;
int32_t fMaxSigDigits;
UBool fUseScientific;
UBool fUseSigDigits;

// Updating any of the following fields triggers an update to
// fEffGrouping field Again we do it this way because original
// grouping settings have to be retained if grouping is turned off.
DigitGrouping fGrouping;
UBool fUseGrouping;

// Updating any of the following fields triggers updates on the following:
// fOptions.fMantissa.fMonetary, fRules, fAffixParser, fSciFormatter, fFormatter
// fAap.fPositivePrefiix, fAap.fPositiveSuffix,
// fAap.fNegativePrefiix, fAap.fNegativeSuffix, and fAap.fWidth
// We do this two phase update because localizing the affix patterns
// and formatters can be expensive. Better to do it once with the setters
// than each time within format.
AffixPattern fPositivePrefixPattern;
AffixPattern fNegativePrefixPattern;
AffixPattern fPositiveSuffixPattern;
AffixPattern fNegativeSuffixPattern;
DecimalFormatSymbols *fSymbols;
UChar fCurr[4];
int32_t fFormatWidth;

// Optional may be NULL
PluralRules *fRules;

// This field is totally hidden from user and is used to derive the affixes
// in fAap below from the four affix patterns above.
AffixPatternParser fAffixParser;

// The actual precision used when formatting
ScientificPrecision fEffPrecision;

// The actual grouping used when formatting
DigitGrouping fEffGrouping;
SciFormatterOptions fOptions;   // Encapsulates fixed precision options
SciFormatter fSciFormatter;
DigitFormatter fFormatter;
DigitAffixesAndPadding fAap;

// Scales for precent or permille symbols
UnicodeString &formatDigitList(
        DigitList &number,
        UnicodeString &appendTo,
        FieldPositionHandler &handler,
        UErrorCode &status) const;

// Does not scale for precent or permille symbols
UnicodeString &formatAdjustedDigitList(
        DigitList &number,
        UnicodeString &appendTo,
        FieldPositionHandler &handler,
        UErrorCode &status) const;

void parsePattern(
        const UnicodeString &pattern, UParseError &perror, UErrorCode &status);

ValueFormatter &prepareValueFormatter(ValueFormatter &vf) const;
int32_t getScale() const;

// Updates everything
void updateAll(UErrorCode &status);

// Updates from changes to first group of attributes
void updatePrecision();

// Updates from changes to second group of attributes
void updateGrouping();

// Updates from changes to third group of attributes
void updateFormatting(int32_t changedFormattingFields, UErrorCode &status);

// Helper functions for updatePrecision
void updatePrecisionForScientific();
void updatePrecisionForFixed();
void extractMinMaxDigits(DigitInterval &min, DigitInterval &max) const;
void extractSigDigits(SignificantDigitInterval &sig) const;

// Helper functions for updateFormatting
void updateFormattingUsesCurrency(int32_t &changedFormattingFields);
void updateFormattingPluralRules(
        int32_t &changedFormattingFields, UErrorCode &status);
void updateFormattingAffixParser(
        int32_t &changedFormattingFields, UErrorCode &status);
void updateFormattingFormatters(
        int32_t &changedFormattingFields);
void updateFormattingLocalizedAffixes(
        int32_t &changedFormattingFields, UErrorCode &status);
void updateFormattingWidth(int32_t &changedFormattingFields);

};


U_NAMESPACE_END

#endif // _DECIMFMT2
//eof