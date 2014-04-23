[p0,p1,p2,p3,p4,p5,p6,p7,p8,p9] = load_all_patients_func();

model0123 = train(vertcat(p0,p1,p2,p3));

save('model0123.mat','model0123');


model1234 = train(vertcat(p1,p2,p3,p4));

save('model1234.mat','model1234');


model2345 = train(vertcat(p2,p3,p4,p5));

save('model2345.mat','model2345');


model3456 = train(vertcat(p3,p4,p5,p6));

save('model3456.mat','model3456');

model4567 = train(vertcat(p4,p5,p6,p7));

save('model4567.mat','model4567');

model5678 = train(vertcat(p5,p6,p7,p8));

save('model5678.mat','model5678');

model6789 = train(vertcat(p6,p7,p8,p9));

save('model6789.mat','model6789');

model01234567 = train(vertcat(p0,p1,p2,p3,p4,p5,p6,p7));

save('model01234567.mat','model01234567');