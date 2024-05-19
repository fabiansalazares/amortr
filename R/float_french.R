#' Float-rate french loan amortization schedule calculation function.
#'
#' @param .amount initial principal amount
#' @param .interest_rate yearly interest rate
#' @param .start_date date of first loan payment
#' @param .end_date date in which the last loan payment is expected. This argument overrides n_payments.
#' @param ,n_payments number of payments expected. This argument will be overridden by a non-NULL end_date argument.
#' @param .frequency number of months in a year in which payments are expected.
#' @param .optional_parameters named vector containing constant columns to be added to the return dataframe
#' @param .truncate_month_day if true, all the days in the return dataframe dates column will be set to zero 
#' @param .interest_rate_schedule dataframe containing dates in the first column, and their corresponding interest rates in the second column. Applicable rates beyond the last row in the dataframe will be exstended to the future until end_date or the date that results from adding .n_payments to .start_date
#' @param .recalculation_frequency number of months until a new amortization schedule is calculated with. 
#' @examples
#' floating_french(amount=10000, interest_rate=0.01, start_date=as.Date("2020-03-19"), end_date=as.Date("2032-03-19"), frequency=4, optional_parameters=c("loan_id"="123"), truncate_month_day=FALSE)
#' floating_french(amount=3000, interest_rate=0.01, start_date=as.Date("2020-03-19"), n_payments=36, frequency=12, truncate_month_day=TRUE)
#' @return a dataframe containing the amortization schedule for the loan described in the arguments

floating_french <- function(
    .amount, # principal amount
    .interest_rate, # interest rate of the loan
    .start_date,  # date in which the loan is taken out, i.e. when the borrower receives the principal amount.
    .end_date = NULL, # final date of the loan
    .n_payments = NULL, # number of periods of the loan, i.e. number of payments from borrower to lender
    .frequency = 12, # number of payments done in every year
    .optional_parameters = NULL, # optional columns to be added to the dataframe
    .truncate_month_day = TRUE # trunk month payment_date to the first day of the month
) {
  floating_french_(
    .amount,
    .interest_rate,
    .start_date,
    .end_date,
    .n_payments,
    .frequency,
    .optional_parameters,
    truncate_month_day
  )
  
}