list.of.packages <- c("ggplot2")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)

library(ggplot2)
options(stringsAsFactors = FALSE)

datastats <- read.csv(file = 'stats.csv')
datastats$Date <- as.Date(as.POSIXct(datastats$Timestamp, origin="1970-01-01"))
date_tab <- table(datastats$Date)
date_frequency = data.frame(Date=names(date_tab), Frequency=as.vector(date_tab))
ggplot(data = date_frequency, aes(x = Date, y = Frequency)) +
geom_bar(stat = "identity", fill = "purple") +
labs(title = "Total daily requests", x = "Date", y = "Daily Requests")

pieplot<-function(col, name)
{
	tab <- table(col)
	frequencies_frame = data.frame(Names=names(tab), Frequency=as.vector(tab))

	slices <- as.vector(frequencies_frame$Frequency)
	lbls <- as.list(frequencies_frame$Names)
	pct <- round(slices/sum(slices)*100)
	lbls <- paste(substr(lbls, 1, 16), pct)
	lbls <- paste(lbls,"%",sep="")
	pie(slices,labels = lbls, col=rainbow(length(lbls)), main=name)
}

pieplot(datastats$Class, "Classified Classes")

pieplot(datastats$Satisfaction, "Satisfaction")

print("Script finished")
