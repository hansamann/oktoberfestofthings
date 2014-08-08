var liftups = {};
var tap_timers = {};

$( document ).ready( function() {
	console.log("DOM ready.");

	sparkSSE();
	
	updateLevel(0, 100); updateLiftups(0, 0);
	updateLevel(1, 100); updateLiftups(1, 0);
	updateLevel(2, 100); updateLiftups(2, 0);
	updateLevel(3, 100); updateLiftups(3, 0);
	updateLevel(4, 100); updateLiftups(4, 0);
	updateLevel(5, 100); updateLiftups(5, 0);
	updateLevel(6, 100); updateLiftups(6, 0);
	updateLevel(7, 100); updateLiftups(7, 0);


	//setInterval(fakeLevels, 5000);
	

} );

function sparkSSE()
{
	if (!localStorage.spark_token)
	{
		var spark_token = prompt("Please enter the Spark Core Access Token: ","");
		if (spark_token == null) //user canceled
		{
			alert("I cannot connect without an access token!");
		}
		else
		{
			localStorage.spark_token = spark_token;
		}
	}

	var url = 'https://api.spark.io/v1/events/mug?access_token=' + localStorage.spark_token;

	var source = new EventSource(url);

	source.addEventListener('mug', function(e) {
	  //console.log(e.data);
	  var msg = JSON.parse(e.data);
	  console.log(msg);
	  var data = JSON.parse(msg.data);
	  

	  if (data.d == 'up')
	  {
	  	if (!liftups[data.id])
	  		liftups[data.id] = 0;

		liftups[data.id]++;

	  	updateLiftups(data.id, liftups[data.id]);
	  }
	  else if (data.d == 'down')
	  {
	  	var left = 1000 - (liftups[data.id] * 30) //assuming 30ml per liftup
	  	var percentage = Math.floor(left/1000 * 100);
	  	updateLevel(data.id, percentage);
	  }
	  else if (data.d == 'tap')
	  {
	  	console.log("Tap at " + data.id + "!");
	  	tap(data.id);
	  }

	}, false);

	source.addEventListener('open', function(e) {
	  console.log("SSE channel open.");
	}, false);

	source.addEventListener('error', function(e) {
	  if (e.readyState == EventSource.CLOSED) {
	    console.log("SSE channel broken.");
	  }
	}, false);	
}

function tap(mugID)
{
	$('#'+mugID).addClass('service');
	
	if (tap_timers[mugID])
		clearTimeout(tap_timers[mugID]);

	tap_timers[mugID] = setTimeout(function(){
		$('#'+mugID).removeClass('service');
	}, 10000);
}

function updateLevel(mugID, percentage)
{
	//@SNK: please check this here: http://priteshgupta.com/2011/07/filling-a-glass-with-water-using-html5/
	//height 235 == 100% full
	var height = 216 / 100 * percentage;
	//$('#'+mugID+' .masskrug .level').css('height', height);

	$('#'+mugID+' .masskrug .level').animate({
		height: height
	}, 1000, "easeInOutExpo", function(){
		if (percentage < 25)
		{
			$('#'+mugID+' .masskrug .level').addClass('level_low');
		}
		else
		{
			$('#'+mugID+' .masskrug .level').removeClass('level_low');
		}	
	});
}

function updateLiftups(mugID, count)
{
	$("#liftups_" + mugID).html(count);	
}

function fakeLevels()
{
	for (var i = 0; i < 8; i++)
	{
		updateLevel(i, Math.floor(Math.random()*100));
	}
}

