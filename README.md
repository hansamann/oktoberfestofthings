Oktoberfest of Things
===========

The Oktoberfest of Things is a community project evangelizing the Internet of Things. This repo contains all code for the Oktoberfest of Things project that hybris GmbH created in order to support this community. 

All images and design files are CC BY-SA-2.0. For the code included, see the LICENSE file. 

The project uses a Spark Core which is connected to 8 pressure sensors. The pressure sensors register if the "Masskrug" (mug) is up or down, plus they register a double-tap and send events via Spark.publish() accordingly. These events are received via the JavaScript engien which connected to the SSE Event source from the Spark cloud. This means that the single HTML page (index.gtpl) is a static page. 

The slick groovy-based web framework Gaelyk is mainly used for future extensibility. 

The web UI is live at http://oktoberfestofthings.appspot.com. It will ask for a Spark Core access token which is saved to the HTML5 local storage without any encryption. 

<img src="http://github.com/hansamann/oktoberfestofthings/blob/master/website/screenshot1.png"/>

<img src="http://github.com/hansamann/oktoberfestofthings/blob/master/website/architecture.png"/>
