%% PTO testing


s=serial('COM9','BaudRate',115200,'InputBufferSize',1024*1024)
fopen(s);


%% IDN?

fwrite(s,'IDN?');
read=s.BytesAvailable;
pause(0.2)

while read~=s.BytesAvailable
    read=s.BytesAvailable;
    pause(0.1);
end

str=sprintf('%s',fread(s,s.BytesAvailable));
disp(str)



%% OSCP settings

fwrite(s,'OSCP:FREQ 50K_:TRIG NORM:EDGE FALL');
read=s.BytesAvailable;
pause(0.2)

while read~=s.BytesAvailable
    read=s.BytesAvailable;
    pause(0.1);
end

str=sprintf('%s',fread(s,s.BytesAvailable));
disp(str)

%% OSCP start single
fwrite(s,'OSCP:SRAT');
read=s.BytesAvailable;
pause(0.2)

while read~=s.BytesAvailable
    read=s.BytesAvailable;
    pause(0.1);
end


disp(['precteno ' num2str(s.BytesAvailable)]);
str=sprintf('%s',fread(s,10));
disp(str)
sig=fread(s,s.BytesAvailable);
plot(sig)
    xlim([0 500]);
    ylim([0 255]);
    
if s.BytesAvailable>0
fread(s,s.BytesAvailable);
end


%% OSCP start normal nebo auto

if s.BytesAvailable>0
fread(s,s.BytesAvailable);
end
fwrite(s,'OSCP:SRAT');

pause(0.2)
figure


while s.BytesAvailable<9
pause(0.01)
end

str=sprintf('%s',fread(s,10));
disp(str);
for ii=1:100
    
    while s.BytesAvailable<500
    pause(0.01);
    end
    
    sig=fread(s,508);
    plot(sig)
    xlim([0 500]);
    ylim([0 255]);
    
  
    
end

disp('done')
fwrite(s,'OSCP:STOP'); 
pause(0.1);
if s.BytesAvailable>0
fread(s,s.BytesAvailable);
end























