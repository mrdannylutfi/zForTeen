/* REXX - Monitor Matrix Computation zIIP Offload Ratios */
parse arg job_name .
if job_name = "" then job_name = "MATBIGEX"

/* Access SDSF log data for the target batch job */
rc = isfcalls('ON')
address SDSF "ISFEXEC ST ("job_name")"

/* Extract CPU metrics to evaluate offload health */
do i = 1 to isfrows
    if jaziipcpu.i > 0 then do
       offload_ratio = (jaziipcpu.i / (jacputime.i + jaziipcpu.i)) * 100
       if offload_ratio < 75.0 then do
          alert_msg = "CRITICAL: Job" job_name "zIIP offload dropped to" format(offload_ratio,2,1)"%"
          address TSO "CONSOLE WTO('"alert_msg"')"
       end
    end
end
rc = isfcalls('OFF')
exit 0
