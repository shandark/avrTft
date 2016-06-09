clear all;

%% R5G6B5

%% 31 %  63 & 31

img1 = imread('wheel1.bmp');


temp = double(double(img1) / double((min(min(img1(:,:,1))) + max(max(img1(:,:,1))))));

temp(:,:,1) = temp(:,:,1) * 31;
temp(:,:,2) = temp(:,:,2) * 63;
temp(:,:,3) = temp(:,:,3) * 31;


result = zeros(size(img1),size(img1),1,'uint32');

result = result + bitshift(uint32(temp(:,:,3)),11,'uint32') + bitshift(uint32(temp(:,:,2)),5,'uint32') + uint32(temp(:,:,1));


fid = fopen('imageRaw.h', 'w');

fprintf(fid, 'static const uint16_t PROGMEM image1[] = {');

for row = 1:size(result,1)
    fprintf(fid, '0x%x,', result(row,:));
    fprintf(fid, '\n');
end

fprintf(fid, '};\n\n\n');



img1 = imread('wheel2.bmp');


temp = double(double(img1) / double((min(min(img1(:,:,1))) + max(max(img1(:,:,1))))));

temp(:,:,1) = temp(:,:,1) * 31;
temp(:,:,2) = temp(:,:,2) * 63;
temp(:,:,3) = temp(:,:,3) * 31;


result = zeros(size(img1),size(img1),1,'uint32');

result = result + bitshift(uint32(temp(:,:,3)),11,'uint32') + bitshift(uint32(temp(:,:,2)),5,'uint32') + uint32(temp(:,:,1));


fprintf(fid, 'static const uint16_t PROGMEM image2[] = {');

for row = 1:size(result,1)
    fprintf(fid, '0x%x,', result(row,:));
    fprintf(fid, '\n');
end

fprintf(fid, '};');

fclose(fid);