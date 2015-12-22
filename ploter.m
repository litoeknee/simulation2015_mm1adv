data = load('mm1data.out');
X = data(:, 2);
Y = data(:, 3);
hold on;
stairs(X, Y);
xlabel('TIME'); ylabel('NUMBER OF CUSTOMERS IN QUEUE');
hold off;