// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// fixed_bullet_
Rcpp::DataFrame fixed_bullet_(int64_t amount, float interest_rate, Date start_date, Nullable<Date> end_date, Nullable<NumericVector> n_payments, int frequency, Nullable<List> optional_parameters, bool truncate_month_day);
RcppExport SEXP _amortr_fixed_bullet_(SEXP amountSEXP, SEXP interest_rateSEXP, SEXP start_dateSEXP, SEXP end_dateSEXP, SEXP n_paymentsSEXP, SEXP frequencySEXP, SEXP optional_parametersSEXP, SEXP truncate_month_daySEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int64_t >::type amount(amountSEXP);
    Rcpp::traits::input_parameter< float >::type interest_rate(interest_rateSEXP);
    Rcpp::traits::input_parameter< Date >::type start_date(start_dateSEXP);
    Rcpp::traits::input_parameter< Nullable<Date> >::type end_date(end_dateSEXP);
    Rcpp::traits::input_parameter< Nullable<NumericVector> >::type n_payments(n_paymentsSEXP);
    Rcpp::traits::input_parameter< int >::type frequency(frequencySEXP);
    Rcpp::traits::input_parameter< Nullable<List> >::type optional_parameters(optional_parametersSEXP);
    Rcpp::traits::input_parameter< bool >::type truncate_month_day(truncate_month_daySEXP);
    rcpp_result_gen = Rcpp::wrap(fixed_bullet_(amount, interest_rate, start_date, end_date, n_payments, frequency, optional_parameters, truncate_month_day));
    return rcpp_result_gen;
END_RCPP
}
// fixed_french_
Rcpp::DataFrame fixed_french_(int64_t amount, float interest_rate, Date start_date, Nullable<Date> end_date, Nullable<NumericVector> n_payments, int frequency, Nullable<List> optional_parameters, bool truncate_month_day);
RcppExport SEXP _amortr_fixed_french_(SEXP amountSEXP, SEXP interest_rateSEXP, SEXP start_dateSEXP, SEXP end_dateSEXP, SEXP n_paymentsSEXP, SEXP frequencySEXP, SEXP optional_parametersSEXP, SEXP truncate_month_daySEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int64_t >::type amount(amountSEXP);
    Rcpp::traits::input_parameter< float >::type interest_rate(interest_rateSEXP);
    Rcpp::traits::input_parameter< Date >::type start_date(start_dateSEXP);
    Rcpp::traits::input_parameter< Nullable<Date> >::type end_date(end_dateSEXP);
    Rcpp::traits::input_parameter< Nullable<NumericVector> >::type n_payments(n_paymentsSEXP);
    Rcpp::traits::input_parameter< int >::type frequency(frequencySEXP);
    Rcpp::traits::input_parameter< Nullable<List> >::type optional_parameters(optional_parametersSEXP);
    Rcpp::traits::input_parameter< bool >::type truncate_month_day(truncate_month_daySEXP);
    rcpp_result_gen = Rcpp::wrap(fixed_french_(amount, interest_rate, start_date, end_date, n_payments, frequency, optional_parameters, truncate_month_day));
    return rcpp_result_gen;
END_RCPP
}
// fixed_german_
Rcpp::DataFrame fixed_german_(double amount, float interest_rate, Date start_date, Nullable<Date> end_date, Nullable<NumericVector> n_payments, int frequency, Nullable<List> optional_parameters, bool truncate_month_day);
RcppExport SEXP _amortr_fixed_german_(SEXP amountSEXP, SEXP interest_rateSEXP, SEXP start_dateSEXP, SEXP end_dateSEXP, SEXP n_paymentsSEXP, SEXP frequencySEXP, SEXP optional_parametersSEXP, SEXP truncate_month_daySEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< double >::type amount(amountSEXP);
    Rcpp::traits::input_parameter< float >::type interest_rate(interest_rateSEXP);
    Rcpp::traits::input_parameter< Date >::type start_date(start_dateSEXP);
    Rcpp::traits::input_parameter< Nullable<Date> >::type end_date(end_dateSEXP);
    Rcpp::traits::input_parameter< Nullable<NumericVector> >::type n_payments(n_paymentsSEXP);
    Rcpp::traits::input_parameter< int >::type frequency(frequencySEXP);
    Rcpp::traits::input_parameter< Nullable<List> >::type optional_parameters(optional_parametersSEXP);
    Rcpp::traits::input_parameter< bool >::type truncate_month_day(truncate_month_daySEXP);
    rcpp_result_gen = Rcpp::wrap(fixed_german_(amount, interest_rate, start_date, end_date, n_payments, frequency, optional_parameters, truncate_month_day));
    return rcpp_result_gen;
END_RCPP
}

RcppExport SEXP run_testthat_tests(SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"_amortr_fixed_bullet_", (DL_FUNC) &_amortr_fixed_bullet_, 8},
    {"_amortr_fixed_french_", (DL_FUNC) &_amortr_fixed_french_, 8},
    {"_amortr_fixed_german_", (DL_FUNC) &_amortr_fixed_german_, 8},
    {"run_testthat_tests", (DL_FUNC) &run_testthat_tests, 1},
    {NULL, NULL, 0}
};

RcppExport void R_init_amortr(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}