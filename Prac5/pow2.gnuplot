# Pablo Antonio Bernabeu Cartagena 20083111-V

set terminal png

##Gráfica del pow2

set xrange [0:32]
set yrange [0:100]

set output "pow2.png"  ## Primera gráfica
set xlabel "Exponent of the number"
set ylabel "Steps"
set title  "Pow2"


plot "pow2.steps" using 1:1 with lines title "pow2(n) = 2* pow2(n-1)",\
     "pow2.steps" using 1:2 with lines title "pow2(n) =  pow2(n/2) * pow2(n/2)",\
     "pow2.steps" using 1:3 with lines title "pow2(n) = pow2(n-1) + pow2(n-1)",\
