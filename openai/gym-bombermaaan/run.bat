
REM --load model_DQN.h5

REM python bombermaaan.py --train --load model_DQN.h5 --replay-memory-size=1000 --target-network-update-freq=10 --avg-val-computation-freq=50 --replay-start-size=50 --test-freq=250 --validation-frames=150 --epsilon=0.4 --min-epsilon=0.01 --epsilon-decrease=0.001 --learning-rate=0.01

python bombermaaan.py --train --replay-memory-size=2000 --target-network-update-freq=10 --avg-val-computation-freq=50 --replay-start-size=50 --test-freq=500 --validation-frames=150 --min-epsilon=0.01 --epsilon-decrease=0.01 --learning-rate=0.01
