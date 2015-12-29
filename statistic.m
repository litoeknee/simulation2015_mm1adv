data = load('mm1data.out');
X = 1:1:109;
Y1 = data(:, 2);
Y2 = data(:, 3);
endtime = data(:, 1)==101;
figure(1);
plot(X, Y1(endtime), '*', X, mean(Y1(endtime)), '+');
mean(Y1(endtime))
grid on;
xlabel('Seed Number'); ylabel('Event list empty at time');
avgdelay = data(:, 1)==101;
figure(2);
plot(X, Y2(avgdelay), '*', X, mean(Y2(avgdelay)), '+');
mean(Y2(avgdelay))
grid on;
xlabel('Seed Number'); ylabel('Average delay in queue');
avgniq = data(:, 1)==102;
figure(3);
plot(X, Y1(avgniq), '*', X, mean(Y1(avgniq)), '+');
mean(Y1(avgniq))
grid on;
xlabel('Seed Number'); ylabel('Average number in queue');
svuti = data(:, 1)==102;
figure(4);
plot(X, Y2(svuti), '*', X, mean(Y2(svuti)), '+');
mean(Y2(svuti))
grid on;
xlabel('Seed Number'); ylabel('Server utilization');
customer = data(:, 1)==103;
figure(5);
plot(X, Y1(customer), '*', X, mean(Y1(customer)), '+');
mean(Y1(customer))
grid on;
xlabel('Seed Number'); ylabel('Number of delays completed');
balking = data(:, 1)==103;
figure(6);
plot(X, Y2(balking), '*', X, mean(Y2(balking)), '+');
mean(Y2(balking))
grid on;
xlabel('Seed Number'); ylabel('Number of customer balking');