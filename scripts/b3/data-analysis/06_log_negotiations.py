# 00 - Session Date                         1       10   Session date
# 01 - Instrument Symbol                   12       50   Instrument identifier
# 02 - Trade Number                        63       10   Trade number
# 03 - Trade Price                         74       20   Trade price
# 04 - Traded Quantity                     95       18   Traded quantity
# 05 - Trade Time                         114       15   Trade time (format HH:MM:SS.NNNNNN)
# 06 - Trade Indicator                    127        1   Trade indicador: 1 - Trade  / 2 - Trade cancelled
# 07 - Buy Order Date                     129       10   Buy order date
# 08 - Sequential Buy Order Number        140       15   Sequential buy order number
# 09 - Secondary Order ID - Buy Order     156       15   Secondary Order ID -  Buy Order.
# 10 - Aggressor Buy Order Indicator      172        1   0 - Neutral (Order was not executed) / 1 - Aggressor / 2 - Passive
# 11 - Sell Order Date                    174       10   Sell order sell date
# 12 - Sequential Sell Order Number       185       15   Sequential sell order number
# 13 - Secondary Order ID - Sell Order    201       15   Secondary Order ID -  Buy Order.
# 14 - Aggressor Sell Order Indicator     217        1   0 - Neutral (Order was not executed) / 1 - Aggressor / 2 - Passive
# 15 - Cross Trade Indicator              219        1   Define if the cross trade was intentional: 1 - Intentional / 0 - Not Intentional
# 16 - Buy Member                         221        8   Entering Firm (Buy Side) - Available from March/2014
# 17 - Sell Member                        230        8   Entering Firm (Sell Side) - Available from March/2014

import os
import matplotlib.pyplot as plt
import numpy as np

path = os.getcwd()

target_data_path = path + '/data/b3/20191220/raw/neg/NEG_BMF_20191220_00.txt'
stock_neg_data = {}
data_line = []


data_line = []
data_plot_x = []
data_plot_y = []

target_stock_symbol = "WING20"

stop_at = 300
count = 0

with open(target_data_path, 'r') as data_file:
    for line in data_file:
        data_line = line.split(";")
        stock_symbol = data_line[1].strip()
        trade_cancelled = data_line[6] == 2

        if trade_cancelled or stock_symbol != target_stock_symbol:
            continue
        count += 1

        stock_price = float(data_line[3])
        trade_time = data_line[5]

        print("Instrument Symbol: {}\nTrade Price: {}\nBuy Order SequentialNumber: {}\nSell Order SequentialNumber: {}\n".format(
            stock_symbol,
            stock_price,
            data_line[8],
            data_line[12]
        ))

        if count == stop_at:
            break

#CPA
#1410805
#1415016 = 000725370159473;000000113855367;001;09:00:08.884000;0934884192; 000000115650.000000;000000000000000020;000000000000000000;2019-12-20;2019-12-20 09:00:08;0;0;00000003
#          000725370159473;000000113857391;004;09:00:46.642000;0934884192; 000000115650.000000;000000000000000020;000000000000000001;2019-12-20;2019-12-20 09:00:46;1;2;00000003
#          000725370159473;000000113857393;004;09:00:46.642000;0934884192; 000000115650.000000;000000000000000020;000000000000000002;2019-12-20;2019-12-20 09:00:46;1;2;00000003
#          000725370159473;000000113857395;004;09:00:46.642000;0934884192; 000000115650.000000;000000000000000020;000000000000000006;2019-12-20;2019-12-20 09:00:46;1;2;00000003
#          000725370159473;000000113857397;004;09:00:46.642000;0934884192; 000000115650.000000;000000000000000020;000000000000000012;2019-12-20;2019-12-20 09:00:46;1;2;00000003
#          000725370159473;000000113857399;004;09:00:46.642000;0934884192; 000000115650.000000;000000000000000020;000000000000000017;2019-12-20;2019-12-20 09:00:46;1;2;00000003
#          000725370159473;000000113857401;004;09:00:46.642000;0934884192; 000000115650.000000;000000000000000020;000000000000000020;2019-12-20;2019-12-20 09:00:46;2;2;00000003


#1907309
#1910403 = 000725370159963;000000113856150;001;09:00:39.528000;0934893552; 000000115635.000000;000000000000000001;000000000000000000;2019-12-20;2019-12-20 09:00:39;0;0;00000308
#          000725370159963;000000113856215;002;09:00:41.018000;0934894240; 000000115630.000000;000000000000000001;000000000000000000;2019-12-20;2019-12-20 09:00:41;5;0;00000308
#          000725370159963;000000113856285;002;09:00:42.742000;0934895056; 000000115625.000000;000000000000000001;000000000000000000;2019-12-20;2019-12-20 09:00:42;5;0;00000308
#          000725370159963;000000113856437;002;09:00:46.354000;0934896736; 000000115615.000000;000000000000000001;000000000000000000;2019-12-20;2019-12-20 09:00:46;5;0;00000308
#          000725370159963;000000113857394;004;09:00:46.642000;0934896736; 000000115615.000000;000000000000000001;000000000000000001;2019-12-20;2019-12-20 09:00:46;2;2;00000308