data = load('mm1data.out');
X = data(:, 1);
Y = data(:, 2);
hold on;
stairs(X, Y);
xlabel('TIME'); ylabel('NUMBER OF CUSTOMERS IN QUEUE');
hold off;