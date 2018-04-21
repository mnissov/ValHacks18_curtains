# -*- coding: utf-8 -*-
"""
Created on Sat Apr 21 14:40:49 2018

@author: morten
"""

#test

from weather import Weather, Unit
weather = Weather(unit=Unit.CELSIUS)

# Lookup WOEID via http://weather.yahoo.com.

lookup = weather.lookup(22126215)
condition = lookup.condition
print(condition.text)

# Get weather forecasts for the upcoming days.

forecasts = lookup.forecast
for forecast in forecasts:
    print(forecast.text)
    print(forecast.date)
    print(forecast.high)
    print(forecast.low)
