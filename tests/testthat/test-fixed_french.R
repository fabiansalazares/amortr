
test_that("total_payment equals interest_payment plus principal_payment for all rows",
          {
            principal_amount = runif(1)*1e2
            interest_rate = runif(1, 0.01, 0.10)
            n_payments = 36
            
            return_df <- fixed_french(amount=principal_amount, 
                                      interest_rate=interest_rate, 
                                      start_date=as.Date("2020-01-01"), 
                                      n_payments=n_payments, 
                                      frequency=12)
            
            row_checks = apply(return_df[,c("total_payment", "interest_payment", "principal_payment")], 
                  1, 
                  function(x) {round(x[[1]] - x[[2]] - x[[3]],4) == 0})
            
            expect_equal(sum(row_checks), length(row_checks))
          })

test_that("end_date argument overrides n_payments.", {
      expect_equal(nrow(
        fixed_french(amount=100, 
                                     interest_rate=0.01, 
                                     start_date=as.Date("2020-01-01"), 
                                     end_date=as.Date("2030-01-01"), 
                                     n_payments=24, 
                                     frequency=12,
                                     NULL,
                                     TRUE))
                   , 12*10)
})

test_that("number of rows equal n_payments", {
  expect_equal(
    nrow(fixed_french(amount=100,
                 interest_rate=0.01,
                 start_date=as.Date("2020-01-01"),
                 n_payments=27,
                 frequency=12
    )),
    27
  )
})

test_that("passing arguments of the wrong type does not crash the session",{
  expect_error( fixed_french(amount=amount_test,
               interest_rate=0.01,
               start_date=131,
               n_payments=as.Date("2020-01-01"),
               frequency=FALSE,
               optional_parameters = TRUE,
               truncate_month_day = 13))
})

test_that("round sum of principal payments equals round initial principal", {
  amount_test<-round(runif(1)*1e3)
  
  expect_equal(
    round(sum(fixed_french(amount=amount_test,
                      interest_rate=0.01,
                      start_date=as.Date("2020-01-01"),
                      n_payments=27,
                      frequency=12)$principal_payment)),
    round(amount_test)
  )})

test_that("sum of principal payments equals initial principal", {
  amount_test<-round(runif(1)*1e3)
  
  expect_equal(
    sum(fixed_french(amount=amount_test,
                           interest_rate=0.01,
                           start_date=as.Date("2020-01-01"),
                           n_payments=27,
                           frequency=12)$principal_payment),
    amount_test
  )
})


