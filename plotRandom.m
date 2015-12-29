data = load('randomnumber.out');
temp = data(:, 2);
n = size(temp, 1)/9;
U = zeros(n, 9);
Umean = zeros(9, 1);
UVar = zeros(9, 1);
Ut = zeros(9, 1);
Uchi2gof = zeros(9, 2);
for i = 1:9
   U(:, i) = temp(1:n*1);
   Umean(i) = mean(U(:, i));
   UVar(i) = var(U(:, i));
   Ut(i) = (Umean(i) - 0.5)/(UVar(i)/sqrt(n - 1));
   [Uchi2gof(i, 1), Uchi2gof(i, 2)] = chi2gof(U(:, i));
end
Umean
UVar
Ut
Uchi2gof
hold on;
plot(U(1:999, i),U(2:1000, i), '*');
xlabel('U(i)'); ylabel('U(i + 1)');
hold off;