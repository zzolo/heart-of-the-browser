/**
 * Main Heart of the Browser visualization code.
 */

(function() {
  // Get window height
  var d = document;
  var e = d.documentElement;
  var g = d.getElementsByTagName('body')[0];
  var w = window.innerWidth || e.clientWidth || g.clientWidth;
  var h = window.innerHeight || e.clientHeight || g.clientHeight;

  // BPM color scale
  var bpmScale = d3.scale.linear()
    .domain([60, 100])
    .range(['#3366FF', '#B82E00']);

  // Connect to SerialBot
  var serialbot = io.connect('//localhost:8899/');

  // Setup canvas
  var svg = d3.select('#heart')
    .append('svg').attr('width', w).attr('height', h);

  // Create circle for heart
  var heart = svg.append('circle')
    .attr('cx', (w / 2))
    .attr('cy', (h / 2))
    .attr('r', Math.min((w / 6), (h / 6)));

  // Setup rickshaw chart
  var signalData = [{
    color: 'red',
    data: []
  }];
  var signal = new Rickshaw.Graph({
    element: document.querySelector('#signal'),
    width: w,
    height: h,
    renderer: 'line',
    min: 100,
    max: 1200,
    series: signalData
  });
  signal.render();

  // Handle data from serial
  serialbot.on('data', function(data) {
    var type = data[0];
    var d = parseInt(data.substring(1), 10);

    if (type == 'B') {
      beat();

      // Color based on beats per minute
      updateBPM(d);
    }
    else if (type == 'S') {
      updateSignal(d);

      // If the signal are very low, no signal or incomplete data
      if (d < 20 || d > 980) {
        validSignal(false);
      }
      else {
        validSignal(true);
      }
    }
  });

  // Update signal
  function updateSignal(data) {
    var limit = 300;

    signalData[0].data.push({
      x: Date.now(),
      y: data
    });

    if (signalData[0].data.length > limit) {
      signalData[0].data.splice(0, signalData[0].data.length - limit);
    }

    signal.update();
  }

  // Do a beat
  function beat() {
    var oR = Math.min((w / 6), (h / 6));
    var nR = Math.min((w / 3), (h / 3));

    heart
      .transition().duration(200).attr('r', nR)
      .transition().duration(400).attr('r', oR);
  }

  // Mark as no signal
  function validSignal(isValid) {
    d3.select('body').classed('valid-signal', isValid);
  }

  // Update BPM coloring
  function updateBPM(bpm) {
    console.log(bpm);
    var c = bpmScale(bpm);

    heart.style('fill', c);
    heart.style('stroke', c);

    signalData[0].color = c;
    signal.update();
  }
})();
