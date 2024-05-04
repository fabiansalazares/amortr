#include <Rcpp.h>
using namespace Rcpp;


Rcpp::DateVector generate_date_range(
    Date start_date,  // date in which the loan is taken out, i.e. when the borrower receives the principal amount.
    Nullable<Date> end_date = R_NilValue, // final date of the loan
    Nullable<NumericVector> n_payments = R_NilValue, // number of periods of the loan, i.e. number of payments from borrower to lender
    int frequency = 12, // number of payments done in every year
    bool truncate_month_day = true // trunk month payment_date to the first day of the month
);